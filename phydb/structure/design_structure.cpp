/*******************************************************************************
 *
 * Copyright (c) 2026 Yihang Yang
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 ******************************************************************************/
#include "design_structure.h"

#include <fstream>
#include <iterator>
#include <sstream>

#include "structure_json.h"

namespace phydb {

namespace {

struct KindName {
  StructureKind kind;
  char const *name;
};

KindName const kKindNames[] = {
    {StructureKind::kRegister, "register"},
    {StructureKind::kVarAccess, "var_access"},
    {StructureKind::kExpression, "expression"},
    {StructureKind::kGuard, "guard"},
    {StructureKind::kDelayLine, "delay_line"},
    {StructureKind::kPortIn, "port_in"},
    {StructureKind::kPortOut, "port_out"},
    {StructureKind::kMerge, "merge"},
};

bool ReadString(StructureJsonValue const &object, char const *key,
                std::string *out) {
  StructureJsonValue const *v = object.Find(key);
  if (v == nullptr || v->type != StructureJsonValue::Type::kString) {
    return false;
  }
  *out = v->string;
  return true;
}

bool ReadInt(StructureJsonValue const &object, char const *key, int *out) {
  StructureJsonValue const *v = object.Find(key);
  if (v == nullptr || v->type != StructureJsonValue::Type::kNumber) {
    return false;
  }
  *out = static_cast<int>(v->number);
  return static_cast<double>(*out) == v->number;
}

}  // namespace

std::string StructureKindName(StructureKind kind) {
  for (auto const &k : kKindNames) {
    if (k.kind == kind) return k.name;
  }
  return "unknown";
}

bool ParseStructureKind(std::string const &name, StructureKind *kind) {
  for (auto const &k : kKindNames) {
    if (name == k.name) {
      *kind = k.kind;
      return true;
    }
  }
  return false;
}

int StructureGraph::AddNode(std::string const &path, StructureKind kind,
                            int width, std::string const &source) {
  auto it = node_index_.find(path);
  if (it != node_index_.end()) return it->second;
  int index = static_cast<int>(nodes_.size());
  StructureNode node;
  node.path = path;
  node.kind = kind;
  node.width = width;
  node.source = source;
  nodes_.push_back(node);
  node_index_[path] = index;
  return index;
}

void StructureGraph::AddEdge(int from, int to) {
  edges_.push_back(StructureGraphEdge{from, to});
}

void StructureGraph::AddMember(int node, std::string const &component_name,
                               int bit) {
  nodes_[node].members.push_back(StructureMember{component_name, bit});
}

void StructureGraph::SetMemberBit(int node, int member, int bit) {
  nodes_[node].members[member].bit = bit;
}

int StructureGraph::FindNode(std::string const &path) const {
  auto it = node_index_.find(path);
  return it == node_index_.end() ? -1 : it->second;
}

void StructureGraph::Clear() {
  nodes_.clear();
  edges_.clear();
  node_index_.clear();
}

/**
 * Parse the synth2 -H schema (version 1):
 *
 *   {"schema_version": 1,
 *    "instances": [{"process_name", "instance_name", "kind", "width",
 *                   "source"}, ...],
 *    "edges":     [{"process_name", "from", "to"}, ...]}
 *
 * Records are grouped into one template per process_name, in file order.
 * Any missing field, unknown kind or wrong type rejects the whole file, so
 * a partially understood structure never reaches the placer.
 */
bool DesignStructure::ReadMetadata(std::string const &file_name,
                                   std::string *error_message) {
  std::ifstream input(file_name);
  if (!input.is_open()) {
    *error_message = "cannot open structure metadata file";
    return false;
  }
  std::string const text((std::istreambuf_iterator<char>(input)),
                         std::istreambuf_iterator<char>());
  StructureJsonValue root;
  if (!ParseStructureJson(text, &root, error_message)) return false;

  int version = 0;
  if (!ReadInt(root, "schema_version", &version) || version != 1) {
    *error_message = "schema_version must be 1";
    return false;
  }
  StructureJsonValue const *instances = root.Find("instances");
  StructureJsonValue const *edges = root.Find("edges");
  if (instances == nullptr ||
      instances->type != StructureJsonValue::Type::kArray ||
      edges == nullptr || edges->type != StructureJsonValue::Type::kArray) {
    *error_message = "instances and edges must be arrays";
    return false;
  }

  std::vector<StructureTemplate> templates;
  std::unordered_map<std::string, int> index;
  auto template_for = [&](std::string const &process) -> StructureTemplate & {
    auto it = index.find(process);
    if (it != index.end()) return templates[it->second];
    index[process] = static_cast<int>(templates.size());
    templates.push_back(StructureTemplate{process, {}, {}});
    return templates.back();
  };

  for (size_t i = 0; i < instances->array.size(); ++i) {
    StructureJsonValue const &rec = instances->array[i];
    std::string process, kind_name;
    StructureTemplateInstance inst;
    if (!ReadString(rec, "process_name", &process) ||
        !ReadString(rec, "instance_name", &inst.instance_name) ||
        !ReadString(rec, "kind", &kind_name) ||
        !ReadInt(rec, "width", &inst.width) ||
        !ReadString(rec, "source", &inst.source) ||
        !ParseStructureKind(kind_name, &inst.kind)) {
      *error_message = "malformed instance record " + std::to_string(i);
      return false;
    }
    template_for(process).instances.push_back(inst);
  }
  for (size_t i = 0; i < edges->array.size(); ++i) {
    StructureJsonValue const &rec = edges->array[i];
    std::string process;
    StructureTemplateEdge edge;
    if (!ReadString(rec, "process_name", &process) ||
        !ReadString(rec, "from", &edge.from) ||
        !ReadString(rec, "to", &edge.to)) {
      *error_message = "malformed edge record " + std::to_string(i);
      return false;
    }
    template_for(process).edges.push_back(edge);
  }

  templates_ = std::move(templates);
  template_index_ = std::move(index);
  graph_.Clear();
  return true;
}

StructureTemplate const *DesignStructure::FindTemplate(
    std::string const &process_name) const {
  auto it = template_index_.find(process_name);
  return it == template_index_.end() ? nullptr : &templates_[it->second];
}

std::string DesignStructure::Summary() const {
  size_t members = 0, with_bit = 0;
  for (auto const &node : graph_.Nodes()) {
    members += node.members.size();
    for (auto const &m : node.members) {
      if (m.bit >= 0) ++with_bit;
    }
  }
  std::ostringstream os;
  os << "design structure: " << templates_.size() << " process templates, "
     << graph_.Nodes().size() << " nodes, " << graph_.Edges().size()
     << " edges, " << members << " member components (" << with_bit
     << " with a bit index)";
  return os.str();
}

bool DesignStructure::WriteReport(std::string const &file_name) const {
  std::ofstream out(file_name);
  if (!out.is_open()) return false;
  out << Summary() << "\n";
  auto const &nodes = graph_.Nodes();
  for (size_t i = 0; i < nodes.size(); ++i) {
    auto const &n = nodes[i];
    out << "node " << i << " " << n.path << " kind=" << StructureKindName(n.kind)
        << " width=" << n.width << " source=" << n.source
        << " members=" << n.members.size() << "\n";
    for (auto const &m : n.members) {
      out << "  member " << m.component_name << " bit=" << m.bit << "\n";
    }
  }
  for (auto const &e : graph_.Edges()) {
    out << "edge " << nodes[e.from].path << " -> " << nodes[e.to].path << "\n";
  }
  return static_cast<bool>(out);
}

}  // namespace phydb

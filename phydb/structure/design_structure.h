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
/**
 * Design structure: what synthesis knows about the circuit, carried to the
 * placer.
 *
 * Synthesis (synth2 -H) records, per synthesized process type, facts it is
 * certain of when it emits an instance: which CHP variable or expression
 * the instance implements, its bit width, and the data-flow edges between
 * instances. Those are StructureTemplates. The loader (interact
 * `phydb:read-structure`) instantiates the templates at every place the
 * process occurs in the expanded ACT hierarchy, producing a
 * StructureGraph whose nodes carry hierarchical instance paths and the
 * phyDB components beneath them, each with its bit index when it has one.
 *
 * Deliberately absent: a datapath/control role. A register can serve a
 * loop counter as easily as data, so role is left for the placer to derive
 * from structure (width, bit membership, data flow).
 *
 * Nothing here is read or written by the existing LEF/DEF/cluster paths:
 * the structure is empty unless explicitly loaded, and loading it changes
 * no other phyDB state.
 */
#ifndef PHYDB_STRUCTURE_DESIGN_STRUCTURE_H_
#define PHYDB_STRUCTURE_DESIGN_STRUCTURE_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace phydb {

enum class StructureKind {
  kRegister,
  kVarAccess,
  kExpression,
  kGuard,
  kDelayLine,
  kPortIn,
  kPortOut,
  /** One-hot merge of a variable written in several branches (a mux). */
  kMerge,
};

/** Name used in the metadata file, e.g. "register". */
std::string StructureKindName(StructureKind kind);

/** Parse a metadata kind name; false when unknown. */
bool ParseStructureKind(std::string const &name, StructureKind *kind);

/** One emitted instance inside a synthesized process, as synthesis saw it. */
struct StructureTemplateInstance {
  std::string instance_name;
  StructureKind kind = StructureKind::kExpression;
  int width = 0;
  std::string source;
};

struct StructureTemplateEdge {
  std::string from;
  std::string to;
};

/** Everything synthesis recorded for one process type. */
struct StructureTemplate {
  std::string process_name;
  std::vector<StructureTemplateInstance> instances;
  std::vector<StructureTemplateEdge> edges;
};

/** A physical cell under a structure node, with its bit (-1: none). */
struct StructureMember {
  std::string component_name;
  int bit = -1;
};

/** A template instance placed at one hierarchical location in the design. */
struct StructureNode {
  std::string path;
  StructureKind kind = StructureKind::kExpression;
  int width = 0;
  std::string source;
  std::vector<StructureMember> members;
};

struct StructureGraphEdge {
  int from = -1;
  int to = -1;
};

/**
 * Structure of the whole design: nodes at hierarchical paths, data-flow
 * edges between them, and the components each node owns.
 */
class StructureGraph {
 public:
  /** Add a node; returns its index, or the existing index for `path`. */
  int AddNode(std::string const &path, StructureKind kind, int width,
              std::string const &source);
  void AddEdge(int from, int to);
  void AddMember(int node, std::string const &component_name, int bit);
  /** Change the bit of member `member` of `node`. */
  void SetMemberBit(int node, int member, int bit);

  /** Index of the node at `path`, or -1. */
  int FindNode(std::string const &path) const;

  std::vector<StructureNode> const &Nodes() const { return nodes_; }
  std::vector<StructureGraphEdge> const &Edges() const { return edges_; }
  bool IsEmpty() const { return nodes_.empty(); }
  void Clear();

 private:
  std::vector<StructureNode> nodes_;
  std::vector<StructureGraphEdge> edges_;
  std::unordered_map<std::string, int> node_index_;
};

/**
 * Container owned by PhyDB: the synthesis templates and the instantiated
 * graph. Empty until ReadMetadata and the loader populate it.
 */
class DesignStructure {
 public:
  /** Parse a synth2 -H file into templates; false with a message on error. */
  bool ReadMetadata(std::string const &file_name, std::string *error_message);

  /**
   * The template for a process type, or nullptr. Matches the exact expanded
   * process name recorded by synthesis (e.g. "decomp_timing_kernel<>").
   */
  StructureTemplate const *FindTemplate(std::string const &process_name) const;

  std::vector<StructureTemplate> const &Templates() const { return templates_; }
  StructureGraph &GraphRef() { return graph_; }
  StructureGraph const &Graph() const { return graph_; }

  /** One-paragraph summary: node, edge and member counts, bit coverage. */
  std::string Summary() const;

  /**
   * Write every node (path, kind, width, source), its members with their
   * bits, and every edge, one per line, for inspection and golden tests.
   */
  bool WriteReport(std::string const &file_name) const;

 private:
  std::vector<StructureTemplate> templates_;
  std::unordered_map<std::string, int> template_index_;
  StructureGraph graph_;
};

}  // namespace phydb

#endif  // PHYDB_STRUCTURE_DESIGN_STRUCTURE_H_

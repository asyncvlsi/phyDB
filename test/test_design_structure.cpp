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
 * Unit test for DesignStructure: parsing synth2 -H metadata, building the
 * graph, and rejecting malformed files without disturbing loaded state.
 *
 * The valid input is an excerpt of what synth2 -H writes for the
 * asymmetric_fork_join kernel (value -> value+1 -> short_path).
 */
#include <cstdio>
#include <fstream>
#include <string>

#include "phydb/phydb.h"

namespace {

int failures = 0;

void Check(bool condition, char const *what) {
  if (!condition) {
    std::fprintf(stderr, "FAIL: %s\n", what);
    ++failures;
  }
}

std::string WriteFile(std::string const &name, std::string const &text) {
  std::ofstream out(name);
  out << text;
  return name;
}

char const *kValid = R"({
  "schema_version": 1,
  "instances": [
    {"process_name": "decomp_timing_kernel<>", "instance_name": "latch_value_0", "kind": "register", "width": 8, "source": "value"},
    {"process_name": "decomp_timing_kernel<>", "instance_name": "va_blk_1", "kind": "var_access", "width": 8, "source": "value"},
    {"process_name": "decomp_timing_kernel<>", "instance_name": "inst_5", "kind": "expression", "width": 8, "source": ""},
    {"process_name": "decomp_timing_kernel<>", "instance_name": "delay_expr_5", "kind": "delay_line", "width": 0, "source": "inst_5"},
    {"process_name": "decomp_sink<>", "instance_name": "latch_value_0", "kind": "register", "width": 8, "source": "value"}
  ],
  "edges": [
    {"process_name": "decomp_timing_kernel<>", "from": "latch_value_0", "to": "va_blk_1"},
    {"process_name": "decomp_timing_kernel<>", "from": "va_blk_1", "to": "inst_5"}
  ]
})";

void TestValidFile() {
  phydb::PhyDB db;
  phydb::DesignStructure &ds = db.GetDesignStructureRef();
  Check(ds.Templates().empty(), "structure starts empty");
  Check(ds.Graph().IsEmpty(), "graph starts empty");

  std::string err;
  bool ok = ds.ReadMetadata(WriteFile("structure_valid.json", kValid), &err);
  Check(ok, "valid file parses");
  Check(ds.Templates().size() == 2, "two process templates");
  phydb::StructureTemplate const *k = ds.FindTemplate("decomp_timing_kernel<>");
  Check(k != nullptr, "kernel template found");
  if (k != nullptr) {
    Check(k->instances.size() == 4, "kernel has 4 instances");
    Check(k->edges.size() == 2, "kernel has 2 edges");
    Check(k->instances[0].kind == phydb::StructureKind::kRegister,
          "first instance is a register");
    Check(k->instances[0].width == 8, "register width 8");
    Check(k->instances[3].source == "inst_5", "delay line names its expression");
  }
  Check(ds.FindTemplate("ring_decomp_timing_kernel<>") == nullptr,
        "no match on an unrecorded name");
}

void TestGraph() {
  phydb::StructureGraph g;
  int a = g.AddNode("kernel_instance.latch_value_0",
                    phydb::StructureKind::kRegister, 8, "value");
  int b = g.AddNode("kernel_instance.va_blk_1",
                    phydb::StructureKind::kVarAccess, 8, "value");
  Check(g.AddNode("kernel_instance.latch_value_0",
                  phydb::StructureKind::kRegister, 8, "value") == a,
        "re-adding a path returns the same node");
  g.AddEdge(a, b);
  g.AddMember(a, "kernel__instance_alatch__value__0_ax1_53_6", 3);
  g.AddMember(a, "kernel__instance_alatch__value__0_aff__ctrl", -1);
  Check(g.Nodes().size() == 2, "two nodes");
  Check(g.Edges().size() == 1, "one edge");
  Check(g.FindNode("kernel_instance.va_blk_1") == b, "find by path");
  Check(g.FindNode("kernel_instance.nothing") == -1, "missing path is -1");
  Check(g.Nodes()[a].members.size() == 2, "two members");
  Check(g.Nodes()[a].members[0].bit == 3, "member keeps its bit");
}

/**
 * Negative controls: each file is wrong in exactly one way and must be
 * rejected, leaving the previously loaded templates in place. The mutation
 * is asserted to be present in the text so a control cannot pass because
 * the edit silently failed.
 */
void TestRejects() {
  struct Case {
    char const *name;
    std::string from;
    std::string to;
  } cases[] = {
      {"unknown kind", "\"kind\": \"var_access\"", "\"kind\": \"datapath\""},
      {"missing width", "\"width\": 8, \"source\": \"value\"}", "\"source\": \"value\"}"},
      {"schema version", "\"schema_version\": 1", "\"schema_version\": 2"},
      {"syntax error", "\"edges\": [", "\"edges\" ["},
  };
  for (auto const &c : cases) {
    std::string text = kValid;
    size_t at = text.find(c.from);
    Check(at != std::string::npos, c.name);
    if (at == std::string::npos) continue;
    text.replace(at, c.from.size(), c.to);
    Check(text != kValid, c.name);

    phydb::DesignStructure ds;
    std::string err;
    Check(ds.ReadMetadata(WriteFile("structure_valid.json", kValid), &err),
          "baseline load before control");
    bool ok = ds.ReadMetadata(WriteFile("structure_bad.json", text), &err);
    Check(!ok, c.name);
    Check(!err.empty(), "rejection carries a message");
    Check(ds.Templates().size() == 2, "rejected file leaves prior state");
  }
  phydb::DesignStructure ds;
  std::string err;
  Check(!ds.ReadMetadata("no_such_structure_file.json", &err),
        "missing file rejected");
}

}  // namespace

int main() {
  TestValidFile();
  TestGraph();
  TestRejects();
  if (failures == 0) std::printf("design structure test: PASS\n");
  return failures == 0 ? 0 : 1;
}

#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#include <sequence.hpp>
#include <utilities/hash.hpp>

// This is a data structure for the dynamic trees problem. The dynamic trees
// problem is to maintain a forest (undirected acyclic graph) as it undergoes
// edge additions ("links") and edge deletions ("cuts").
class DynamicForest {
 public:
  // Initializes forest with `num_vertices` vertices and no edges.
  //
  // Efficiency: Linear in the size of the forest.
  explicit DynamicForest(int64_t num_vertices);
  ~DynamicForest();

  DynamicForest() = delete;
  DynamicForest(const DynamicForest &other) = delete;
  DynamicForest& operator=(const DynamicForest& other) = delete;
  DynamicForest(DynamicForest&& other) noexcept = delete;
  DynamicForest& operator=(DynamicForest&& other) noexcept = delete;

  // Returns true if vertices `u` and `v` are connected, i.e. are in the same
  // tree.
  //
  // Efficiency: Logarithmic in the size of the forest.
  bool IsConnected(int64_t u, int64_t v);

  // Adds edge between vertices `u` and `v`.
  //
  // Adding this edge must not create a cycle in the forest.
  //
  // Efficiency: Logarithmic in the size of the forest.
  void AddEdge(int64_t u, int64_t v);

  // Removes edge between vertices `u` and `v`.
  //
  // The edge (`u`, `v`) must be in the forest.
  //
  // Efficiency: Logarithmic in the size of the forest.
  void DeleteEdge(int64_t u, int64_t v);

 private:
  void AssertVertexInBounds(int64_t v);

  int64_t num_vertices_;
  std::vector<sequence::Element> vertices_;
  // We preallocate all sequence elements for edges in `edge_elements_` and
  // maintain a list of unused elements in `free_edge_elements_`. The used
  // elements are stored in `edges_`, which maps an edge (a pair of vertices) to
  // a sequence element in `edge_elements_`.
  std::vector<sequence::Element> edge_elements_;
  std::vector<sequence::Element*> free_edge_elements_;
  std::unordered_map<
    std::pair<int64_t, int64_t>,
    sequence::Element*,
    IntPairHasher> edges_;
};

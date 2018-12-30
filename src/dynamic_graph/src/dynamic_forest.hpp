#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#include <sequence.hpp>
#include <dynamic_graph/graph.hpp>

namespace detail {

// This is for holding elements for a pair of directed edges (u, v) and (v, u).
struct UndirectedEdgeElements {
  UndirectedEdgeElements(
      sequence::Element* _forward_edge,
      sequence::Element* _backward_edge);
  UndirectedEdgeElements() = delete;

  sequence::Element* const forward_edge;
  sequence::Element* const backward_edge;
};

}  // namespace detail

// This is a data structure for the dynamic trees problem. The dynamic trees
// problem is to maintain a forest (undirected acyclic graph) as it undergoes
// edge additions ("links") and edge deletions ("cuts").
//
// The implementation is specialized for use in Holm et al.'s dynamic
// connectivity algorithm, which is why we have the functions `MarkEdge()` and
// `MarkVertex()`.
class DynamicForest {
 public:
  // Initializes forest with `num_vertices` vertices and no edges.
  //
  // Efficiency: linear in the size of the forest.
  explicit DynamicForest(int64_t num_vertices);
  DynamicForest() = delete;

  ~DynamicForest();

  // Only copies forests that have no edges.
  DynamicForest(const DynamicForest& other);
  DynamicForest& operator=(const DynamicForest& other) = delete;

  DynamicForest(DynamicForest&& other) noexcept;
  DynamicForest& operator=(DynamicForest&& other) noexcept = delete;

  // Returns true if vertices `u` and `v` are connected, i.e. are in the same
  // tree.
  //
  // Efficiency: logarithmic in the size of the forest.
  bool IsConnected(Vertex u, Vertex v) const;

  // Returns true if the edge is in the forest.
  //
  // Efficiency: Constant.
  bool HasEdge(const UndirectedEdge& edge) const;

  // Adds edge to forest.
  //
  // Adding this edge must not create a cycle in the forest.
  //
  // Efficiency: logarithmic in the size of the forest.
  void AddEdge(const UndirectedEdge& edge);

  // Removes edge from forest.
  //
  // The edge must be in the forest.
  //
  // Efficiency: logarithmic in the size of the forest.
  void DeleteEdge(const UndirectedEdge& edge);

  // Returns the number of vertices in the tree that vertex `v` resides in.
  //
  // Efficiency: logarithmic in the size of the forest.
  int64_t GetSizeOfTree(Vertex v) const;

  // Mark (if `mark` is true) or unmark (if `mark` is false) an edge. See
  // `GetMarkedEdgeInTree`.
  //
  // Efficiency: logarithmic in the size of the forest.
  void MarkEdge(const UndirectedEdge& edge, bool mark);
  // Analagous to `GetEdge`.
  void MarkVertex(Vertex v, bool mark);
  // Finds an edge in the tree that vertex `v` resides in that was marked by
  // `MarkEdge(., ., true)`.
  //
  // Efficiency: logarithmic in the size of the forest.
  std::optional<UndirectedEdge> GetMarkedEdgeInTree(Vertex v) const;
  // Analagous to `GetMarkedVertexInTree`.
  std::optional<Vertex> GetMarkedVertexInTree(Vertex v) const;

 private:
  detail::UndirectedEdgeElements
  AllocateEdgeElements(const UndirectedEdge& edge);
  void FreeEdgeElements(const detail::UndirectedEdgeElements& edge_elements);

  const int64_t num_vertices_;
  std::vector<sequence::Element> vertices_;
  // We preallocate all sequence elements for edges in `edge_elements_` and
  // maintain a list of unused elements in `free_edge_elements_`. The used
  // elements are stored in `edges_`, which maps an undirected edge to sequence
  // elements in `edge_elements_`.
  std::vector<sequence::Element> edge_elements_;
  std::vector<sequence::Element*> free_edge_elements_;
  // Maps undirected edge {u, v} to elements representing directed edges (u, v)
  // and (v, u).
  std::unordered_map<
    UndirectedEdge,
    detail::UndirectedEdgeElements,
    UndirectedEdgeHash> edges_;
};

/** @file dynamic_connectivity.hpp
 *  Declaration for a data structure that maintains connectivity information on
 *  an undirected graph that can have edges added or removed.
 *
 *  @author Tom Tseng (tomtseng)
 */
#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include <dynamic_forest.hpp>

/** This class represents an undirected graph that can undergo efficient edge
 *  insertions, edge deletions, and connectivity queries.
 */
class DynamicConnectivity {
 public:
  /** Initializes an empty graph with a fixed number of vertices.
   *
   *  Efficiency: \f$ O(n \log n ) \f$ where \f$ n \f$ is the number of vertices
   *  in the graph.
   *
   *  @param[in] num_vertices Number of vertices in the graph.
   */
  explicit DynamicConnectivity(int64_t num_vertices);

  /** Deallocates the data structure. */
  ~DynamicConnectivity();

  /** The default constructor is invalid. */
  DynamicConnectivity() = delete;
  /** Copy constructor not implemented. */
  DynamicConnectivity(const DynamicConnectivity& other) = delete;
  /** Copy assignment not implemented. */
  DynamicConnectivity& operator=(const DynamicConnectivity& other) = delete;
  /** Move constructor not implemented. */
  DynamicConnectivity(DynamicConnectivity&& other) noexcept = delete;
  /** Move assignment not implemented. */
  DynamicConnectivity& operator=(DynamicConnectivity&& other) noexcept = delete;

  /** Returns true if vertices \p u and \p v are connected in the graph.
   *
   *  Efficiency: logarithmic in the size of the graph.
   *
   *  @param[in] u Vertex.
   *  @param[in] v Vertex.
   *  @returns True if \p u and \p v are connected, false if they are not.
   */
  bool IsConnected(Vertex u, Vertex v) const;

  /** Returns the number of vertices in `v`'s connected component.
   *
   * Efficiency: logarithmic in the size of the graph.
   *
   * @param[in] v Vertex.
   * @returns The number of vertices in \p v's connected component.
   */
  int64_t GetSizeOfConnectedComponent(Vertex v) const;

  /** Adds an edge to the graph.
   *
   *  The edge must not already be in the graph.
   *
   *  Efficiency: \f$ O\left( \log^2 n \right) \f$ amortized where \f$ n \f$ is
   *  the number of vertices in the graph.
   *
   *  @param[in] edge Edge to be added.
   */
  void AddEdge(const UndirectedEdge& edge);

  /** Deletes an edge from the graph.
   *
   *  The edge must be in the graph.
   *
   *  Efficiency: \f$ O\left( \log^2 n \right) \f$ amortized where \f$ n \f$ is
   *  the number of vertices in the graph.
   *
   *  @param[in] edge Edge to be deleted.
   */
  void DeleteEdge(const UndirectedEdge& edge);

 private:
  const int64_t num_vertices_;
  // `spanning_forests_[i]` stores F_i, the spanning forest for the i-th
  // subgraph.
  std::vector<DynamicForest> spanning_forests_;
  // `adjacency_lists_by_level_[i][v]` contains the vertices connected to vertex
  // v by level-i non-tree edges (edges not in the spanning forests).
  std::vector<std::vector<std::vector<Vertex>>>
    non_tree_adjacency_lists_by_level_;
  // Maps non-tree edges to their levels.
  std::unordered_map<UndirectedEdge, int8_t, UndirectedEdgeHash>
    non_tree_edge_to_level_;
};

/** \file dynamic_connectivity.hpp
 * Header file for a data structure that maintains connectivity information on
 * an undirected graph that can have edges added or removed.
 *
 * @author Tom Tseng (tomtseng)
 */
#pragma once

#include <cstdint>

/**
 * This class represents an undirected graph that can undergo efficient edge
 * insertions, edge deletions, and connectivity queries.
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
  DynamicConnectivity(const DynamicConnectivity &other) = delete;
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
  bool IsConnected(int64_t u, int64_t v) const;

  /** Returns the number of vertices in `v`'s connected component.
   *
   * Efficiency: logarithmic in the size of the graph.
   *
   * @param[in] v Vertex.
   * @returns The number of vertices in \p v's connected component.
   */
  int64_t GetSizeOfConnectedComponent(int64_t v) const;

  /** Adds an edge between vertices \p u and \p v.
   *
   *  The edge must not already be in the graph.
   *
   *  Efficiency: logarithmic in the size of the graph.
   *
   *  @param[in] u Vertex.
   *  @param[in] v Vertex.
   */
  void AddEdge(int64_t u, int64_t v);

  /** Deletes the edge between vertices \p u and \p v.
   *
   *  The edge must be in the graph.
   *
   *  Efficiency: logarithmic in the size of the graph.
   *
   *  @param[in] u Vertex.
   *  @param[in] v Vertex.
   */
  void DeleteEdge(int64_t u, int64_t v);

 private:
  int64_t num_vertices_;
};

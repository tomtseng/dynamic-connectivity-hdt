// TODO header comment
// TODO Doxygen
#pragma once

#include <cstdint>

class DynamicConnectivity {
 public:
  // Efficiency: O(n * log(n)) where n is the number of vertices in the graph.
  explicit DynamicConnectivity(int64_t num_vertices);
  ~DynamicConnectivity();

  DynamicConnectivity() = delete;
  DynamicConnectivity(const DynamicConnectivity &other) = delete;
  DynamicConnectivity& operator=(const DynamicConnectivity& other) = delete;
  DynamicConnectivity(DynamicConnectivity&& other) noexcept = delete;
  DynamicConnectivity& operator=(DynamicConnectivity&& other) noexcept = delete;

  // Returns true if vertices `u` and `v` are connected.
  //
  // Efficiency: logarithmic in the size of the graph.
  bool IsConnected(int64_t u, int64_t v) const;

  // Returns the number of vertices in `v`'s connected component.
  //
  // Efficiency: logarithmic in the size of the forest.
  int64_t GetSizeOfConnectedComponent(int64_t v) const;

  // Adds edge between vertices `u` and `v`.
  //
  // The edge must not already be in the graph.
  //
  // Efficiency: logarithmic in the size of the graph.
  void AddEdge(int64_t u, int64_t v);

  // Removes edge between vertices `u` and `v`.
  //
  // The edge must be in the graph.
  //
  // Efficiency: logarithmic in the size of the graph.
  void DeleteEdge(int64_t u, int64_t v);

 private:
  int64_t num_vertices_;
};

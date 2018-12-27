#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#include "sequence.hpp"
#include "utilities.hpp"

// TODO comments
class DynamicTrees {
 public:
  explicit DynamicTrees(uint32_t num_vertices);
  ~DynamicTrees();

  DynamicTrees() = delete;
  DynamicTrees(const DynamicTrees &other) = delete;
  DynamicTrees& operator=(const DynamicTrees& other) = delete;
  DynamicTrees(DynamicTrees&& other) noexcept = delete;
  DynamicTrees& operator=(DynamicTrees&& other) noexcept = delete;

  bool IsConnected(uint32_t u, uint32_t v);
  void Link(uint32_t u, uint32_t v);
  void Cut(uint32_t u, uint32_t v);

 private:
  uint32_t num_vertices_;
  // TODO add comment explaining this
  std::vector<sequence::Element> edge_elements_;
  std::vector<sequence::Element*> free_edge_elements_;
  std::vector<sequence::Element> vertices_;
  std::unordered_map<
    std::pair<uint32_t, uint32_t>,
    sequence::Element*,
    IntPairHasher> edges_;
};

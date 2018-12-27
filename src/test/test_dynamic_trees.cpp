#include "dynamic_trees.hpp"

#include <gtest/gtest.h>

TEST(DynamicTrees, LinkAndCutPathGraph) {
  constexpr uint32_t kNumVertices = 10;
  DynamicTrees dynamic_trees(kNumVertices);
  for (uint32_t i = 1; i < kNumVertices; i++) {
    dynamic_trees.Link(i - 1, i);
  }
  for (uint32_t i = 1; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_trees.IsConnected(0, i));
  }

  dynamic_trees.Cut(4, 5);
  EXPECT_FALSE(dynamic_trees.IsConnected(4, 5));
  for (uint32_t i = 0; i < 4; i++) {
    EXPECT_TRUE(dynamic_trees.IsConnected(4, i));
  }
  for (uint32_t i = 5; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_trees.IsConnected(5, i));
  }

  for (uint32_t i = 1; i < kNumVertices; i++) {
    if (i != 5) {
      dynamic_trees.Cut(i - 1, i);
    }
  }
  for (uint32_t i = 0; i < kNumVertices; i++) {
    for (uint32_t j = i + 1; j < kNumVertices; j++) {
      EXPECT_FALSE(dynamic_trees.IsConnected(i, j));
    }
  }
}

TEST(DynamicTrees, LinkAndCutStarGraph) {
  constexpr uint32_t kNumVertices = 10;
  DynamicTrees dynamic_trees(kNumVertices);
  for (uint32_t i = 1; i < kNumVertices; i++) {
    dynamic_trees.Link(0, i);
  }
  for (uint32_t i = 1; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_trees.IsConnected(0, i));
  }

  dynamic_trees.Cut(0, 5);
  for (uint32_t i = 0; i < kNumVertices; i++) {
    EXPECT_EQ(dynamic_trees.IsConnected(0, i), i != 5);
  }

  for (uint32_t i = 1; i < kNumVertices; i++) {
    if (i != 5) {
      dynamic_trees.Cut(0, i);
    }
  }
  for (uint32_t i = 0; i < kNumVertices; i++) {
    for (uint32_t j = i + 1; j < kNumVertices; j++) {
      EXPECT_FALSE(dynamic_trees.IsConnected(i, j));
    }
  }
}

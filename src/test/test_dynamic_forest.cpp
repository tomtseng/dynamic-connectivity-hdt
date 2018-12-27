#include "dynamic_forest.hpp"

#include <gtest/gtest.h>

TEST(DynamicForest, AddEdgeAndDeleteEdgePathGraph) {
  constexpr uint64_t kNumVertices = 10;
  DynamicForest dynamic_forest(kNumVertices);
  for (uint64_t i = 1; i < kNumVertices; i++) {
    dynamic_forest.AddEdge(i - 1, i);
  }
  for (uint64_t i = 1; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(0, i));
  }

  dynamic_forest.DeleteEdge(4, 5);
  EXPECT_FALSE(dynamic_forest.IsConnected(4, 5));
  for (uint64_t i = 0; i < 4; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(4, i));
  }
  for (uint64_t i = 5; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(5, i));
  }

  for (uint64_t i = 1; i < kNumVertices; i++) {
    if (i != 5) {
      dynamic_forest.DeleteEdge(i - 1, i);
    }
  }
  for (uint64_t i = 0; i < kNumVertices; i++) {
    for (uint64_t j = i + 1; j < kNumVertices; j++) {
      EXPECT_FALSE(dynamic_forest.IsConnected(i, j));
    }
  }
}

TEST(DynamicForest, AddEdgeAndDeleteEdgeStarGraph) {
  constexpr uint64_t kNumVertices = 10;
  DynamicForest dynamic_forest(kNumVertices);
  for (uint64_t i = 1; i < kNumVertices; i++) {
    dynamic_forest.AddEdge(0, i);
  }
  for (uint64_t i = 1; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(0, i));
  }

  dynamic_forest.DeleteEdge(0, 5);
  for (uint64_t i = 0; i < kNumVertices; i++) {
    EXPECT_EQ(dynamic_forest.IsConnected(0, i), i != 5);
  }

  for (uint64_t i = 1; i < kNumVertices; i++) {
    if (i != 5) {
      dynamic_forest.DeleteEdge(0, i);
    }
  }
  for (uint64_t i = 0; i < kNumVertices; i++) {
    for (uint64_t j = i + 1; j < kNumVertices; j++) {
      EXPECT_FALSE(dynamic_forest.IsConnected(i, j));
    }
  }
}

#include "dynamic_forest.hpp"

#include <gtest/gtest.h>

TEST(DynamicForest, LinkAndCutPathGraph) {
  constexpr uint32_t kNumVertices = 10;
  DynamicForest dynamic_forest(kNumVertices);
  for (uint32_t i = 1; i < kNumVertices; i++) {
    dynamic_forest.Link(i - 1, i);
  }
  for (uint32_t i = 1; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(0, i));
  }

  dynamic_forest.Cut(4, 5);
  EXPECT_FALSE(dynamic_forest.IsConnected(4, 5));
  for (uint32_t i = 0; i < 4; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(4, i));
  }
  for (uint32_t i = 5; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(5, i));
  }

  for (uint32_t i = 1; i < kNumVertices; i++) {
    if (i != 5) {
      dynamic_forest.Cut(i - 1, i);
    }
  }
  for (uint32_t i = 0; i < kNumVertices; i++) {
    for (uint32_t j = i + 1; j < kNumVertices; j++) {
      EXPECT_FALSE(dynamic_forest.IsConnected(i, j));
    }
  }
}

TEST(DynamicForest, LinkAndCutStarGraph) {
  constexpr uint32_t kNumVertices = 10;
  DynamicForest dynamic_forest(kNumVertices);
  for (uint32_t i = 1; i < kNumVertices; i++) {
    dynamic_forest.Link(0, i);
  }
  for (uint32_t i = 1; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(0, i));
  }

  dynamic_forest.Cut(0, 5);
  for (uint32_t i = 0; i < kNumVertices; i++) {
    EXPECT_EQ(dynamic_forest.IsConnected(0, i), i != 5);
  }

  for (uint32_t i = 1; i < kNumVertices; i++) {
    if (i != 5) {
      dynamic_forest.Cut(0, i);
    }
  }
  for (uint32_t i = 0; i < kNumVertices; i++) {
    for (uint32_t j = i + 1; j < kNumVertices; j++) {
      EXPECT_FALSE(dynamic_forest.IsConnected(i, j));
    }
  }
}

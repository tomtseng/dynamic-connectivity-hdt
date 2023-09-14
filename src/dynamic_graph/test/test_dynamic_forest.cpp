#include <dynamic_forest.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Optional;

TEST(DynamicForest, AddEdgeAndDeleteEdgePathGraph) {
  constexpr int64_t kNumVertices = 10;
  DynamicForest dynamic_forest(kNumVertices);
  for (int64_t i = 1; i < kNumVertices; i++) {
    dynamic_forest.AddEdge({i - 1, i});
  }
  for (int64_t i = 1; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(0, i));
  }

  dynamic_forest.DeleteEdge({4, 5});
  EXPECT_FALSE(dynamic_forest.IsConnected(4, 5));
  for (int64_t i = 0; i < 4; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(4, i));
  }
  for (int64_t i = 5; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(5, i));
  }

  for (int64_t i = 1; i < kNumVertices; i++) {
    if (i != 5) {
      dynamic_forest.DeleteEdge({i - 1, i});
    }
  }
  for (int64_t i = 0; i < kNumVertices; i++) {
    for (int64_t j = i + 1; j < kNumVertices; j++) {
      EXPECT_FALSE(dynamic_forest.IsConnected(i, j));
    }
  }
}

TEST(DynamicForest, AddEdgeAndDeleteEdgeStarGraph) {
  constexpr int64_t kNumVertices = 10;
  DynamicForest dynamic_forest(kNumVertices);
  for (int64_t i = 1; i < kNumVertices; i++) {
    dynamic_forest.AddEdge({0, i});
  }
  for (int64_t i = 1; i < kNumVertices; i++) {
    EXPECT_TRUE(dynamic_forest.IsConnected(0, i));
  }

  dynamic_forest.DeleteEdge({0, 5});
  for (int64_t i = 0; i < kNumVertices; i++) {
    EXPECT_EQ(dynamic_forest.IsConnected(0, i), i != 5);
  }

  for (int64_t i = 1; i < kNumVertices; i++) {
    if (i != 5) {
      dynamic_forest.DeleteEdge({0, i});
    }
  }
  for (int64_t i = 0; i < kNumVertices; i++) {
    for (int64_t j = i + 1; j < kNumVertices; j++) {
      EXPECT_FALSE(dynamic_forest.IsConnected(i, j));
    }
  }
}

TEST(DynamicForest, Mark) {
  DynamicForest dynamic_forest(10);

  dynamic_forest.MarkVertex(8, true);
  for (int32_t i = 1; i < 10; i++) {
    dynamic_forest.AddEdge({i - 1, i});
  }
  EXPECT_FALSE(dynamic_forest.GetMarkedEdgeInTree(0).has_value());
  EXPECT_THAT(dynamic_forest.GetMarkedVertexInTree(0), Optional(8));

  dynamic_forest.MarkEdge({2, 3}, true);
  EXPECT_THAT(
      dynamic_forest.GetMarkedEdgeInTree(0),
      Optional(UndirectedEdge(2, 3)));

  dynamic_forest.MarkEdge({6, 7}, true);
  dynamic_forest.DeleteEdge({2, 3});
  EXPECT_FALSE(dynamic_forest.GetMarkedEdgeInTree(0).has_value());
  EXPECT_FALSE(dynamic_forest.GetMarkedVertexInTree(0).has_value());
  EXPECT_THAT(
      dynamic_forest.GetMarkedEdgeInTree(9),
      Optional(UndirectedEdge(6, 7)));
  EXPECT_THAT(dynamic_forest.GetMarkedVertexInTree(9), Optional(8));

  dynamic_forest.MarkEdge({6, 7}, false);
  EXPECT_FALSE(dynamic_forest.GetMarkedEdgeInTree(9).has_value());

  dynamic_forest.MarkVertex(8, false);
  dynamic_forest.MarkVertex(1, true);
  EXPECT_FALSE(dynamic_forest.GetMarkedVertexInTree(9).has_value());
  EXPECT_THAT(dynamic_forest.GetMarkedVertexInTree(0), Optional(1));

  dynamic_forest.MarkEdge({6, 7}, true);
  dynamic_forest.AddEdge({3, 2});
  EXPECT_THAT(
      dynamic_forest.GetMarkedEdgeInTree(0),
      Optional(UndirectedEdge(6, 7)));
  EXPECT_THAT(dynamic_forest.GetMarkedVertexInTree(9), Optional(1));

  dynamic_forest.MarkEdge({6, 7}, false);
  EXPECT_FALSE(dynamic_forest.GetMarkedEdgeInTree(0).has_value());
}

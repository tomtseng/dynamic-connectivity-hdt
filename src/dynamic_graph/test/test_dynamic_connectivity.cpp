#include <dynamic_graph/dynamic_connectivity.hpp>

#include <gtest/gtest.h>

TEST(DynamicConnectivity, SingleVertexGraph) {
  DynamicConnectivity graph(1);
  EXPECT_TRUE(graph.IsConnected(0, 0));
}

TEST(DynamicConnectivity, AddAndDeleteEdge) {
  DynamicConnectivity graph(6);

  // Graph is two triangles:
  //   0          5
  //   |\        /|
  //   | \      / |
  //   2--1    4--3
  graph.AddEdge(UndirectedEdge(0, 1));
  graph.AddEdge(UndirectedEdge(1, 2));
  graph.AddEdge(UndirectedEdge(2, 0));
  graph.AddEdge(UndirectedEdge(3, 4));
  graph.AddEdge(UndirectedEdge(4, 5));
  graph.AddEdge(UndirectedEdge(5, 3));
  EXPECT_TRUE(graph.IsConnected(0, 2));
  EXPECT_TRUE(graph.IsConnected(3, 5));
  EXPECT_FALSE(graph.IsConnected(0, 5));

  // Add a couple of edges between the triangles, then delete them.
  graph.AddEdge(UndirectedEdge(2, 4));
  EXPECT_TRUE(graph.IsConnected(0, 5));
  graph.AddEdge(UndirectedEdge(1, 4));
  EXPECT_TRUE(graph.IsConnected(0, 5));
  graph.DeleteEdge(UndirectedEdge(2, 4));
  EXPECT_TRUE(graph.IsConnected(0, 5));
  graph.DeleteEdge(UndirectedEdge(1, 4));
  EXPECT_FALSE(graph.IsConnected(0, 5));

  // Add all edges between the triangles, then delete them.
  graph.AddEdge(UndirectedEdge(0, 3));
  graph.AddEdge(UndirectedEdge(0, 4));
  graph.AddEdge(UndirectedEdge(0, 5));
  graph.AddEdge(UndirectedEdge(1, 3));
  graph.AddEdge(UndirectedEdge(1, 4));
  graph.AddEdge(UndirectedEdge(1, 5));
  graph.AddEdge(UndirectedEdge(2, 3));
  graph.AddEdge(UndirectedEdge(2, 4));
  graph.AddEdge(UndirectedEdge(2, 5));
  EXPECT_TRUE(graph.IsConnected(0, 5));
  graph.DeleteEdge(UndirectedEdge(0, 3));
  graph.DeleteEdge(UndirectedEdge(0, 4));
  graph.DeleteEdge(UndirectedEdge(0, 5));
  graph.DeleteEdge(UndirectedEdge(1, 3));
  graph.DeleteEdge(UndirectedEdge(1, 4));
  graph.DeleteEdge(UndirectedEdge(1, 5));
  graph.DeleteEdge(UndirectedEdge(2, 3));
  graph.DeleteEdge(UndirectedEdge(2, 4));
  EXPECT_TRUE(graph.IsConnected(0, 5));
  graph.DeleteEdge(UndirectedEdge(2, 5));
  EXPECT_FALSE(graph.IsConnected(0, 5));

  // Delete a few edges from one triangle.
  graph.DeleteEdge(UndirectedEdge(0, 2));
  EXPECT_TRUE(graph.IsConnected(0, 2));
  graph.DeleteEdge(UndirectedEdge(0, 1));
  EXPECT_FALSE(graph.IsConnected(0, 2));
  EXPECT_TRUE(graph.IsConnected(1, 2));
}

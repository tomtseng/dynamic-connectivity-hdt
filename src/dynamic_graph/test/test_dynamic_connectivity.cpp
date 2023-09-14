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
  graph.AddEdge({0, 1});
  graph.AddEdge({1, 2});
  graph.AddEdge({2, 0});
  graph.AddEdge({3, 4});
  graph.AddEdge({4, 5});
  graph.AddEdge({5, 3});
  EXPECT_TRUE(graph.IsConnected(0, 2));
  EXPECT_TRUE(graph.IsConnected(3, 5));
  EXPECT_FALSE(graph.IsConnected(0, 5));

  // Add a couple of edges between the triangles, then delete them.
  graph.AddEdge({2, 4});
  EXPECT_TRUE(graph.IsConnected(0, 5));
  graph.AddEdge({1, 4});
  EXPECT_TRUE(graph.IsConnected(0, 5));
  graph.DeleteEdge({2, 4});
  EXPECT_TRUE(graph.IsConnected(0, 5));
  graph.DeleteEdge({1, 4});
  EXPECT_FALSE(graph.IsConnected(0, 5));

  // Add all edges between the triangles, then delete them.
  graph.AddEdge({0, 3});
  graph.AddEdge({0, 4});
  graph.AddEdge({0, 5});
  graph.AddEdge({1, 3});
  graph.AddEdge({1, 4});
  graph.AddEdge({1, 5});
  graph.AddEdge({2, 3});
  graph.AddEdge({2, 4});
  graph.AddEdge({2, 5});
  EXPECT_TRUE(graph.IsConnected(0, 5));
  graph.DeleteEdge({0, 3});
  graph.DeleteEdge({0, 4});
  graph.DeleteEdge({0, 5});
  graph.DeleteEdge({1, 3});
  graph.DeleteEdge({1, 4});
  graph.DeleteEdge({1, 5});
  graph.DeleteEdge({2, 3});
  graph.DeleteEdge({2, 4});
  EXPECT_TRUE(graph.IsConnected(0, 5));
  graph.DeleteEdge({2, 5});
  EXPECT_FALSE(graph.IsConnected(0, 5));

  // Delete a few edges from one triangle.
  graph.DeleteEdge({0, 2});
  EXPECT_TRUE(graph.IsConnected(0, 2));
  graph.DeleteEdge({0, 1});
  EXPECT_FALSE(graph.IsConnected(0, 2));
  EXPECT_TRUE(graph.IsConnected(1, 2));
}

TEST(DynamicConnectivity, GetSizeOfConnectedComponent) {
  DynamicConnectivity graph(4);
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 1);

  graph.AddEdge({0, 1});
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 2);
  graph.AddEdge({1, 2});
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 3);
  graph.AddEdge({0, 2});
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 3);
  graph.AddEdge({0, 3});
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 4);
  graph.AddEdge({1, 3});
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 4);

  graph.DeleteEdge({0, 2});
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 4);
  graph.DeleteEdge({1, 2});
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 3);
  graph.DeleteEdge({0, 3});
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 3);
  graph.DeleteEdge({0, 1});
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 2);
  graph.DeleteEdge({1, 3});
  EXPECT_EQ(graph.GetSizeOfConnectedComponent(1), 1);
}

TEST(DynamicConnectivity, GetNumberOfConnectedComponents) {
  DynamicConnectivity graph(4);
  EXPECT_EQ(graph.GetNumberOfConnectedComponents(), 4);

  graph.AddEdge({0, 1});
  EXPECT_EQ(graph.GetNumberOfConnectedComponents(), 3);
  graph.AddEdge({2, 3});
  EXPECT_EQ(graph.GetNumberOfConnectedComponents(), 2);
  graph.AddEdge({0, 2});
  EXPECT_EQ(graph.GetNumberOfConnectedComponents(), 1);
  graph.AddEdge({1, 3});
  EXPECT_EQ(graph.GetNumberOfConnectedComponents(), 1);

  graph.DeleteEdge({0, 1});
  EXPECT_EQ(graph.GetNumberOfConnectedComponents(), 1);
  graph.DeleteEdge({2, 3});
  EXPECT_EQ(graph.GetNumberOfConnectedComponents(), 2);
  graph.DeleteEdge({0, 2});
  EXPECT_EQ(graph.GetNumberOfConnectedComponents(), 3);
  graph.DeleteEdge({1, 3});
  EXPECT_EQ(graph.GetNumberOfConnectedComponents(), 4);
}

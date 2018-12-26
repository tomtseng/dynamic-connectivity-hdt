#include <dynamic_connectivity/treap.hpp>

#include <gtest/gtest.h>

TEST(Treap, DummyTest) {
  treap::Node nodes[2];
  EXPECT_NE(nodes[0].GetRoot(), nodes[1].GetRoot());
  treap::Node::Join(&nodes[0], &nodes[1]);
  EXPECT_EQ(nodes[0].GetRoot(), nodes[1].GetRoot());
  nodes[0].Split();
  EXPECT_NE(nodes[0].GetRoot(), nodes[1].GetRoot());
}

TEST(Treap, DummyFailingTest) {
  EXPECT_EQ(1, 2);
}

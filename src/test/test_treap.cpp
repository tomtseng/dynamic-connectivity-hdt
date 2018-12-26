#include <dynamic_connectivity/treap.hpp>

#include <gtest/gtest.h>

TEST(Treap, JoinAndSplitSingleNodes) {
  treap::Node nodes[2];
  EXPECT_NE(nodes[0].GetRoot(), nodes[1].GetRoot());

  treap::Node* join_result = treap::Node::Join(&nodes[0], &nodes[1]);
  EXPECT_EQ(nodes[0].GetRoot(), nodes[1].GetRoot());
  EXPECT_EQ(nodes[0].GetRoot(), join_result->GetRoot());

  std::pair<treap::Node*, treap::Node*> split_result = nodes[0].Split();
  EXPECT_NE(nodes[0].GetRoot(), nodes[1].GetRoot());
  EXPECT_EQ(&nodes[0], split_result.first);
  EXPECT_EQ(&nodes[1], split_result.second);
}

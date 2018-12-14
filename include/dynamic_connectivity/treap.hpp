// The treap is a binary tree data structure that has logarithmic height with
// respect to the size of the tree with high probability.
#pragma once

#include <cstdint>
#include <utility>

namespace treap {

// Usage: create single-element trees with the `Node()` constructor, and build
// bigger trees from there.
class Node {
 public:
  Node();
  ~Node();

  // Don't allow copying or moving.
  // Document these functions if you implement them. Does copying copy the whole
  // tree? Is it the user's responsibility to deallocate the copied tree?
  Node(const Node &other) = delete;
  Node& operator=(const Node& other) = delete;
  Node(Node&& other) noexcept = delete;
  Node& operator=(Node&& other) noexcept = delete;

  // Returns the root of the tree that this node lives in.
  Node* GetRoot() const;

  // Splits the tree T that this node lives in immediately after the node.
  //
  // Returns a pair of roots {u, v} such that
  // * u's tree contains this node as well as all elements that originally were
  //   before this node in the in-order traversal of T, and
  // * v's tree contains all elements that were after this node.
  std::pair<Node*, Node*> Split();

  // Joins the tree U containing `lesser` to the tree V containing `greater` and
  // returns the root of resulting tree. The result contains all elements of U
  // in order followed by all elements of V in order.
  static Node* Join(Node* lesser, Node* greater);

 private:
  void AssignChild(bool direction, Node* v);
  static Node* JoinRoots(Node* lesser, Node* greater);

  Node* children_[2];
  Node* parent_;
  // Treap invariant: the priority of a node must be at least as great as the
  // priority of each of its children.
  uint32_t priority_;
};

}  // namespace treap

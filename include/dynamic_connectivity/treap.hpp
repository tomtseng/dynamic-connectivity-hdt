// TODO(tomtseng) Doxygen
#pragma once

#include <cstdint>
#include <utility>

namespace treap {

class Node {
 public:
  Node();

  Node* GetRoot() const;

  // Splits right after this node.
  std::pair<Node*, Node*> Split();
  // Join tree containing lesser to tree containing greater and return root of
  // resulting tree.
  static Node* Join(Node* lesser, Node* greater);

 private:
  void AssignChild(int i, Node* v);
  static Node* JoinRoots(Node* lesser, Node* greater);

  Node* parent_;
  Node* child_[2];
  uint32_t priority_;
};

}  // namespace treap

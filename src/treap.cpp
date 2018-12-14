#include <dynamic_connectivity/treap.hpp>

#include <random>
#include <stdexcept>

namespace treap {

namespace {

  constexpr bool LEFT = 0;
  constexpr bool RIGHT = 1;

  std::mt19937 random_generator{0};
  std::uniform_int_distribution<uint32_t> priority_distribution{0, UINT32_MAX};

}  // namespace

Node::Node()
  : children_{nullptr, nullptr}
  , parent_{nullptr}
  , priority_{priority_distribution(random_generator)} {}

Node::~Node() {}

void Node::AssignChild(bool direction, Node* v) {
  if (v != nullptr) {
    v->parent_ = this;
  }
  children_[direction] = v;
}

Node* Node::GetRoot() const {
  const Node* current{this};
  while (current->parent_ != nullptr) {
    current = current->parent_;
  }
  return const_cast<Node*>(current);
}

std::pair<Node*, Node*> Node::Split() {
  Node* lesser{nullptr};
  Node* greater{children_[RIGHT]};
  if (children_[RIGHT] != nullptr) {
    children_[RIGHT]->parent_ = nullptr;
    AssignChild(RIGHT, nullptr);
  }

  Node* current{this};
  bool traversed_up_from_right{true};
  bool current_is_right_child{true};
  while (current != nullptr) {
    Node* parent{current->parent_};
    if (parent != nullptr) {
      current_is_right_child = parent->children_[RIGHT] == current;
      parent->AssignChild(current_is_right_child, nullptr);
      current->parent_ = nullptr;
    }
    if (traversed_up_from_right) {
      lesser = Join(current, lesser);
    } else {
      greater = Join(greater, current);
    }

    traversed_up_from_right = current_is_right_child;
    current = parent;
  }
  return {lesser, greater};
}

// Joins the tree rooted at `lesser` to the tree rooted at `greater`.
Node* Node::JoinRoots(Node* lesser, Node* greater) {
  if (lesser == nullptr) {
    return greater;
  } else if (greater == nullptr) {
    return lesser;
  }

  if (lesser->priority_ > greater->priority_) {
    lesser->AssignChild(RIGHT, JoinRoots(lesser->children_[RIGHT], greater));
    return lesser;
  } else {
    greater->AssignChild(LEFT, JoinRoots(lesser, greater->children_[LEFT]));
    return greater;
  }
}

Node* Node::Join(Node* lesser, Node* greater) {
  Node* const lesser_root = lesser == nullptr ? nullptr : lesser->GetRoot();
  Node* const greater_root = greater == nullptr ? nullptr : greater->GetRoot();
  if (lesser_root == greater_root && lesser_root != nullptr) {
    throw std::invalid_argument("Input nodes live in the same tree");
  }
  return JoinRoots(
      lesser == nullptr ? nullptr : lesser->GetRoot(),
      greater == nullptr ? nullptr : greater->GetRoot());
}

}  // namespace treap

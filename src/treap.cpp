#include <dynamic_connectivity/treap.hpp>

#include <random>

namespace treap {

namespace {

  constexpr int LEFT = 0;
  constexpr int RIGHT = 1;

  std::mt19937 random_generator{0};
  std::uniform_int_distribution<uint32_t> priority_distribution{0, UINT32_MAX};

}  // namespace

Node::Node()
  : parent_{nullptr}
  , child_{nullptr, nullptr}
  , priority_{priority_distribution(random_generator)} {}

void Node::AssignChild(int i, Node* v) {
  if (v != nullptr) {
    v->parent_ = this;
  }
  child_[i] = v;
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
  Node* greater{child_[RIGHT]};
  if (child_[RIGHT] != nullptr) {
    child_[RIGHT]->parent_ = nullptr;
    AssignChild(RIGHT, nullptr);
  }

  Node* current{this};
  bool traversed_up_from_right{true};
  bool current_is_right_child{true};
  while (current != nullptr) {
    Node* parent{current->parent_};
    if (parent != nullptr) {
      current_is_right_child = parent->child_[RIGHT] == current;
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

Node* Node::JoinRoots(Node* lesser, Node* greater) {
  if (lesser == nullptr) {
    return greater;
  } else if (greater == nullptr) {
    return lesser;
  }

  if (lesser->priority_ > greater->priority_) {
    lesser->AssignChild(RIGHT, JoinRoots(lesser->child_[RIGHT], greater));
    return lesser;
  } else {
    greater->AssignChild(LEFT, JoinRoots(lesser, greater->child_[LEFT]));
    return greater;
  }
}

Node* Node::Join(Node* lesser, Node* greater) {
  return JoinRoots(
      lesser == nullptr ? nullptr : lesser->GetRoot(),
      greater == nullptr ? nullptr : greater->GetRoot());
}

}  // namespace treap

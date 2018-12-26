// This is implemented as a treap. The treap is a binary tree data structure
// that has logarithmic height with respect to the size of the tree with high
// probability. Each element in the sequence is a treap node, and the in-order
// traversal of the treap gives the sequence elements in order.
#include <dynamic_connectivity/sequence.hpp>

#include <random>
#include <stdexcept>

namespace sequence {

namespace {

  constexpr bool LEFT = 0;
  constexpr bool RIGHT = 1;

  std::mt19937 random_generator{0};
  std::uniform_int_distribution<uint32_t> priority_distribution{0, UINT32_MAX};

}  // namespace

Element::Element()
  : children_{nullptr, nullptr}
  , parent_{nullptr}
  , priority_{priority_distribution(random_generator)} {}

Element::~Element() {}

void Element::AssignChild(bool direction, Element* child) {
  if (child != nullptr) {
    child->parent_ = this;
  }
  children_[direction] = child;
}

Element* Element::GetRoot() const {
  const Element* current{this};
  while (current->parent_ != nullptr) {
    current = current->parent_;
  }
  return const_cast<Element*>(current);
}

Element* Element::GetRepresentative() const {
  return GetRoot();
}

// Joins the tree rooted at `lesser` to the tree rooted at `greater`.
Element* Element::JoinRoots(Element* lesser, Element* greater) {
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

// Joins the tree that `lesser` lives in with the tree that `greater` lives in
// and returns the root of the resulting tree.
Element* Element::JoinWithRootReturned(Element* lesser, Element* greater) {
  Element* const lesser_root = lesser == nullptr ? nullptr : lesser->GetRoot();
  Element* const greater_root =
    greater == nullptr ? nullptr : greater->GetRoot();
  if (lesser_root == greater_root && lesser_root != nullptr) {
    throw std::invalid_argument("Input nodes live in the same sequence");
  }
  return JoinRoots(
      lesser == nullptr ? nullptr : lesser->GetRoot(),
      greater == nullptr ? nullptr : greater->GetRoot());
}

void Element::Join(Element* lesser, Element* greater) {
  JoinWithRootReturned(lesser, greater);
}

std::pair<Element*, Element*> Element::Split() {
  Element* lesser{nullptr};
  Element* greater{children_[RIGHT]};
  if (children_[RIGHT] != nullptr) {
    children_[RIGHT]->parent_ = nullptr;
    AssignChild(RIGHT, nullptr);
  }

  Element* current{this};
  bool traversed_up_from_right{true};
  bool current_is_right_child{true};
  while (current != nullptr) {
    Element* parent{current->parent_};
    if (parent != nullptr) {
      current_is_right_child = parent->children_[RIGHT] == current;
      parent->AssignChild(current_is_right_child, nullptr);
      current->parent_ = nullptr;
    }
    if (traversed_up_from_right) {
      lesser = JoinWithRootReturned(current, lesser);
    } else {
      greater = JoinWithRootReturned(greater, current);
    }

    traversed_up_from_right = current_is_right_child;
    current = parent;
  }
  return {lesser, greater};
}

}  // namespace sequence

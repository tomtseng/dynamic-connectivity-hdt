// This is implemented as a treap. The treap is a binary tree data structure
// that has logarithmic height with respect to the number of elements in the
// tree with high probability. Each element in the sequence is a treap node, and
// the in-order traversal of the treap gives the sequence elements in order.
#include <sequence/sequence.hpp>

#include <limits>
#include <random>
#include <stdexcept>

#include <utilities/assert.hpp>

namespace sequence {

namespace {

  constexpr bool kLeft = 0;
  constexpr bool kRight = 1;

  std::mt19937 random_generator{0};
  std::uniform_int_distribution<uint32_t> priority_distribution{
    0, std::numeric_limits<uint32_t>::max()};

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
    lesser->AssignChild(kRight, JoinRoots(lesser->children_[kRight], greater));
    return lesser;
  } else {
    greater->AssignChild(kLeft, JoinRoots(lesser, greater->children_[kLeft]));
    return greater;
  }
}

// Joins the tree that `lesser` lives in with the tree that `greater` lives in
// and returns the root of the resulting tree.
Element* Element::JoinWithRootReturned(Element* lesser, Element* greater) {
  Element* const lesser_root = lesser == nullptr ? nullptr : lesser->GetRoot();
  Element* const greater_root =
    greater == nullptr ? nullptr : greater->GetRoot();
  ASSERT_MSG(
      lesser_root != greater_root || lesser_root == nullptr,
      "Input nodes live in the same sequence");
  return JoinRoots(
      lesser == nullptr ? nullptr : lesser->GetRoot(),
      greater == nullptr ? nullptr : greater->GetRoot());
}

void Element::Join(Element* lesser, Element* greater) {
  JoinWithRootReturned(lesser, greater);
}

Element* Element::Split() {
  // `lesser` is the root of a sequence that will contain `this` and elements
  // preceding `this`.
  Element* lesser{nullptr};
  // `greater`is the root of a sequence that will contain all elements
  // after `this`.
  Element* greater{children_[kRight]};
  if (children_[kRight] != nullptr) {
    children_[kRight]->parent_ = nullptr;
    AssignChild(kRight, nullptr);
  }

  Element* current{this};
  bool traversed_up_from_right{true};
  bool current_is_right_child{true};
  while (current != nullptr) {
    Element* parent{current->parent_};
    if (parent != nullptr) {
      current_is_right_child = parent->children_[kRight] == current;
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

  // The former successor of `this` is the leftmost descendent of `greater`.
  Element* successor = greater;
  while (successor != nullptr && successor->children_[kLeft] != nullptr) {
    successor = successor->children_[kLeft];
  }
  return successor;
}

Element* Element::GetPredecessor() const {
  const Element* current{this};
  if (current->children_[kLeft] == nullptr) {
    // No left child. The predecessor is the first ancestor for which the start
    // element falls in the ancestor's right subtree.
    while (true) {
      if (current->parent_ == nullptr) {
        return nullptr;
      } else if (current->parent_->children_[kRight] == current) {
        return current->parent_;
      } else {
        current = current->parent_;
      }
    }
  } else {
    // The element has a left child. The predecessor is the right-most node in
    // the left child's subtree.
    current = current->children_[kLeft];
    while (current->children_[kRight] != nullptr) {
      current = current->children_[kRight];
    }
    return const_cast<Element*>(current);
  }
}

}  // namespace sequence

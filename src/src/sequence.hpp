#pragma once

#include <cstdint>
#include <utility>

namespace sequence {

// Usage: create single-element sequences with the `Element()` constructor, and
// build bigger sequences from there.
class Element {
 public:
  Element();
  ~Element();

  // Don't allow copying or moving.
  // Document these functions if you implement them. Does copying copy the whole
  // sequence? Is it the user's responsibility to deallocate the copied
  // sequence?
  Element(const Element &other) = delete;
  Element& operator=(const Element& other) = delete;
  Element(Element&& other) noexcept = delete;
  Element& operator=(Element&& other) noexcept = delete;

  // Returns a representative of the sequence that the element lives in.
  //
  // Two elements are in the same sequence if and only if their representatives
  // are the same. Representatives are invalidated after the sequence is
  // modified.
  Element* GetRepresentative() const;

  // Concatenates the sequence containing `lesser` and the sequence containing
  // `greater`.
  //
  // `lesser` and `greater` must not live in the same sequence.
  static void Join(Element* lesser, Element* greater);

  // Splits the sequence that this element lives in immediately after the
  // element.
  //
  // Returns a pair of elements {u, v} such that
  // * u's sequence contains this element and all elements that were before this
  //   element, and
  // * v's sequence contains all elements that were after this element.
  std::pair<Element*, Element*> Split();

 private:
  void AssignChild(bool direction, Element* child);
  Element* GetRoot() const;
  static Element* JoinRoots(Element* lesser, Element* greater);
  static Element* JoinWithRootReturned(Element* lesser, Element* greater);

  Element* children_[2];
  Element* parent_;
  // Treap invariant: the priority of a node must be at least as great as the
  // priority of each of its children.
  uint32_t priority_;
};

}  // namespace sequence

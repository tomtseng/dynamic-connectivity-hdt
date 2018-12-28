#pragma once

#include <cstdint>
#include <array>
#include <utility>

namespace sequence {

// Usage: create single-element sequences with the `Element()` constructor, and
// build bigger sequences from there.
class Element {
 public:
  // Initializes a single sequence element.
  Element(const std::pair<int64_t, int64_t>& id);
  Element();

  ~Element();

  // Copies elements that are in single-element sequences. Copying will throw an
  // exception if attempted on an element that lives in a sequence of several
  // elements.
  Element(const Element &other);
  Element& operator=(const Element& other) = delete;  // unimplemented
  // Moves element. Other elements that point to the moved element will be
  // changed to point to the new element.
  Element(Element&& other) noexcept;
  Element& operator=(Element&& other) noexcept = delete;  // unimplemented

  // Returns a representative of the sequence that the element lives in.
  //
  // Two elements are in the same sequence if and only if their representatives
  // are the same. Representatives are invalidated after the sequence is
  // modified.
  //
  // Efficiency: Logarithmic in the size of the element's sequence.
  Element* GetRepresentative() const;

  // Concatenates the sequence containing `lesser` and the sequence containing
  // `greater`.
  //
  // `lesser` and `greater` must not live in the same sequence.
  //
  // Efficiency: Logarithmic in the sum of the sizes of `lesser` and `greater`'s
  // sequences.
  static void Join(Element* lesser, Element* greater);

  // Splits the sequence that this element lives in immediately after the
  // element.
  //
  // After splitting, this element's sequence contains itself and all elements
  // that were before this element, and the returned element's sequence contains
  // all elements that were after the calling element.
  //
  // Returns what was formerly the successor of this element.
  //
  // Efficiency: Logarithmic in the size of the element's sequence.
  Element* Split();

  // Get element immediately preceding this element in the sequence. Returns
  // null if this element is the first element in the sequence.
  //
  // Efficiency: Logarithmic in the size of the element's sequence.
  Element* GetPredecessor() const;

  // Identifier for the element.
  //
  // This is specialized for storing Euler tour elements. The identifier can
  // store what edge this element represents.
  std::pair<int64_t, int64_t> id_{-1, -1};

 private:
  void AssignChild(bool direction, Element* child);
  Element* GetRoot() const;
  static Element* JoinRoots(Element* lesser, Element* greater);
  static Element* JoinWithRootReturned(Element* lesser, Element* greater);

  std::array<Element*, 2> children_{nullptr, nullptr};
  Element* parent_{nullptr};
  // Treap invariant: the priority of a node must be at least as great as the
  // priority of each of its children.
  int64_t priority_;
};

}  // namespace sequence

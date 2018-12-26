#include <sequence.hpp>

#include <gtest/gtest.h>

namespace seq = sequence;

TEST(Sequence, JoinAndSplitSingleElements) {
  seq::Element elements[2];
  EXPECT_NE(elements[0].GetRepresentative(), elements[1].GetRepresentative());

  seq::Element::Join(&elements[0], &elements[1]);
  EXPECT_EQ(elements[0].GetRepresentative(), elements[1].GetRepresentative());

  std::pair<seq::Element*, seq::Element*> split_result = elements[0].Split();
  EXPECT_NE(elements[0].GetRepresentative(), elements[1].GetRepresentative());
  EXPECT_EQ(&elements[0], split_result.first);
  EXPECT_EQ(&elements[1], split_result.second);
}

TEST(Sequence, JoinAndSplitEmptySequences) {
  // Check that joining with empty sequences and splitting at the end of a
  // sequence doesn't cause errors.

  seq::Element::Join(nullptr, nullptr);

  seq::Element elements[2];
  seq::Element::Join(&elements[0], &elements[1]);
  seq::Element::Join(&elements[1], nullptr);
  seq::Element::Join(nullptr, &elements[1]);
  elements[1].Split();
  EXPECT_EQ(elements[0].GetRepresentative(), elements[1].GetRepresentative());
}

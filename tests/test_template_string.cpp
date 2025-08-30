#include "Maxwell.hpp"

#include <algorithm>
#include <compare>
#include <gtest/gtest.h>
#include <ranges>

using namespace maxwell::utility;

TEST(TestTemplateString, TestRangeProperties) {
  EXPECT_TRUE(std::ranges::range<template_string<2>>);
  EXPECT_TRUE(std::ranges::sized_range<template_string<2>>);
  EXPECT_TRUE(std::ranges::contiguous_range<template_string<2>>);
}

TEST(TestTemplateString, TestConstructor) {
  const template_string str{"Hello"};

  EXPECT_EQ(str.size(), 5);

  const std::array<char, 5> expected{'H', 'e', 'l', 'l', 'o'};
  std::array<char, 5> test;
  std::ranges::copy(str, test.begin());

  EXPECT_EQ(expected, test);
}

TEST(TestTemplateString, TestCompare) {
  const template_string str1{"Hello"};
  const template_string str2{"Hello"};
  const template_string str3{"Zello"};

  EXPECT_EQ(str1 <=> str2, std::strong_ordering::equal);
  EXPECT_EQ(str2 <=> str3, std::strong_ordering::less);
  EXPECT_EQ(str3 <=> str2, std::strong_ordering::greater);
}

TEST(TestTemplateString, TestEquality) {
  const template_string str1{"Hello"};
  const template_string str2{"Hello"};
  const template_string str3{"World"};
  const template_string str4{"Worl"};

  EXPECT_EQ(str1, str2);
  EXPECT_NE(str2, str3);
  EXPECT_NE(str3, str4);
}

TEST(TestTemplateString, TestConcatenation) {
  const template_string str1{"Hello"};
  const template_string str2{"World"};
  const template_string str3{"HelloWorld"};

  EXPECT_EQ(str1 + str2, str3);
}

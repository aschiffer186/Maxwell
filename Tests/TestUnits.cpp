#include <gtest/gtest.h>

#include "Maxwell.hh"

using namespace Maxwell;

#include <array>

TEST(TestUnits, TestUnitBase)
{
    EXPECT_TRUE(UnitBaseLike<decltype(NullUnitBase)>);
    EXPECT_FALSE(UnitBaseLike<int>);

    EXPECT_TRUE(isCoherentUnitBase(CoherentUnitBase));
    EXPECT_TRUE(isCoherentUnitBase(UnitBase<2, 1, 1, 1, 1, 1>{}));
    EXPECT_FALSE(isCoherentUnitBase(UnitBase<2, 2, 1, 1, 1, 1>{}));

    EXPECT_TRUE(CoherentUnitBase == CoherentUnitBase);
    EXPECT_FALSE(CoherentUnitBase != NullUnitBase);
}

TEST(TestUnits, TestUnit) {}
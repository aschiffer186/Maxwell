#include "gtest/gtest.h"
#include <gtest/gtest.h>

#include "Maxwell.hpp"

#include <string>

using namespace Maxwell;

class Custom {
  public:
    static int numCopyCtorCalls;
    static int numMoveCtorCalls;

    Custom() : d_{} {}

    explicit Custom(double d) : d_(d) {}

    Custom(const Custom& c) : d_(c.d_) { ++numCopyCtorCalls; }

    Custom(Custom&& c) noexcept(false) : d_(c.d_) { ++numMoveCtorCalls; }

    friend auto operator==(const Custom&,
                           const Custom&) noexcept -> bool = default;

  private:
    double d_;
};

int Custom::numCopyCtorCalls{0};
int Custom::numMoveCtorCalls{0};

auto
operator+(Custom, Custom) -> Custom {
    return {};
}

auto
operator-(Custom, Custom) -> Custom {
    return {};
}

auto
operator*(Custom, Custom) -> Custom {
    return {};
}

auto
operator/(Custom, Custom) -> Custom {
    return {};
}

auto
operator*(double, Custom) -> Custom {
    return {};
}

class Custom2 {
  public:
    Custom2() : d_{} {}

    explicit Custom2(double d) : d_(d) {}

    Custom2(const Custom2& c) = delete;

    Custom2(Custom2&& c) = default;

    friend auto operator==(const Custom2&,
                           const Custom2&) noexcept -> bool = default;

    ~Custom2() {}

  private:
    double d_;
};

auto
operator+(Custom2, Custom2) -> Custom2 {
    return {};
}

auto
operator-(Custom2, Custom2) -> Custom2 {
    return {};
}

auto
operator*(Custom2, Custom2) -> Custom2 {
    return {};
}

auto
operator/(Custom2, Custom2) -> Custom2 {
    return {};
}

auto
operator*(double, Custom2) -> Custom2 {
    return {};
}

template <typename T> class TestQuantityFixture : public testing::Test {
  protected:
    T value_;
};

using MagnitudeTypes = ::testing::Types<unsigned, int, double, Custom>;
TYPED_TEST_SUITE(TestQuantityFixture, MagnitudeTypes);

TYPED_TEST(TestQuantityFixture, TestProperties) {
    using QuantityType = BasicQuantity<TypeParam, MeterUnit>;

    EXPECT_EQ(noexcept(QuantityType()), noexcept(TypeParam()));
    EXPECT_EQ(sizeof(QuantityType), sizeof(TypeParam));
    EXPECT_EQ(alignof(QuantityType), alignof(TypeParam));
    EXPECT_EQ(std::is_trivially_destructible_v<QuantityType>,
              std::is_trivially_destructible_v<TypeParam>);
    EXPECT_EQ(std::is_copy_constructible_v<QuantityType>,
              std::is_copy_constructible_v<TypeParam>);
    EXPECT_EQ(std::is_nothrow_copy_constructible_v<QuantityType>,
              std::is_nothrow_copy_constructible_v<TypeParam>);
    EXPECT_EQ(std::is_move_constructible_v<QuantityType>,
              std::is_move_constructible_v<TypeParam>);
    EXPECT_EQ(std::is_nothrow_move_constructible_v<QuantityType>,
              std::is_nothrow_move_constructible_v<TypeParam>);
}

TYPED_TEST(TestQuantityFixture, TestDefaultConstructor) {
    using QuantityType = BasicQuantity<TypeParam, MeterUnit>;

    QuantityType q;
    EXPECT_EQ(q.magnitude(), TypeParam());
    EXPECT_EQ(q.units(), MeterUnit);
    EXPECT_EQ(std::is_nothrow_default_constructible_v<QuantityType>,
              std::is_nothrow_default_constructible_v<TypeParam>);
}

TYPED_TEST(TestQuantityFixture, TestSingleArgumentConstructor) {
    using QuantityType = BasicQuantity<Custom, MeterUnit>;

    const int initCopyCtorCalls = Custom::numCopyCtorCalls;
    const int initMoveCtorCalls = Custom::numMoveCtorCalls;

    Custom c{1.0};
    QuantityType q1{c};
    EXPECT_EQ(q1.magnitude(), Custom{1.0});
    EXPECT_EQ(q1.units(), MeterUnit);
    EXPECT_EQ(Custom::numCopyCtorCalls, initCopyCtorCalls + 1);
    EXPECT_EQ(Custom::numMoveCtorCalls, initMoveCtorCalls);

    QuantityType q2{Custom{1.0}};
    EXPECT_EQ(q1.magnitude(), Custom{1.0});
    EXPECT_EQ(q1.units(), MeterUnit);
    EXPECT_EQ(Custom::numCopyCtorCalls, initCopyCtorCalls + 1);
    EXPECT_EQ(Custom::numMoveCtorCalls, initMoveCtorCalls + 1);

    using QuantityType2 = BasicQuantity<TypeParam, MeterUnit>;

    EXPECT_EQ(
        (std::is_nothrow_constructible_v<QuantityType2, const TypeParam&>),
        (std::is_nothrow_constructible_v<TypeParam, const TypeParam&>) );

    EXPECT_EQ((std::is_nothrow_constructible_v<QuantityType2, TypeParam&&>),
              (std::is_nothrow_constructible_v<TypeParam, TypeParam&&>) );
}

#include "gtest/gtest.h"
#include <concepts>
#include <gtest/gtest.h>

#include <numbers>
#include <vector>

#include "Maxwell.hpp"

using namespace Maxwell;

class Custom {
  public:
    static int numCopyCtorCalls;
    static int numMoveCtorCalls;

    Custom() : d_{} {}

    explicit Custom(double d) : d_(d) {}

    Custom(const Custom& c) : d_(c.d_) { ++numCopyCtorCalls; }

    Custom(Custom&& c) noexcept(false) : d_(c.d_) { ++numMoveCtorCalls; }

    friend auto operator==(const Custom&, const Custom&) noexcept -> bool = default;

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

    friend auto operator==(const Custom2&, const Custom2&) noexcept -> bool = default;

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

auto
operator+(const std::vector<double>&, const std::vector<double>&) -> std::vector<double> {
    return {};
}

auto
operator-(const std::vector<double>&, const std::vector<double>&) -> std::vector<double> {
    return {};
}

auto
operator*(const std::vector<double>&, const std::vector<double>&) -> std::vector<double> {
    return {};
}

auto
operator*(double, const std::vector<double>&) -> std::vector<double> {
    return {};
}

auto
operator/(const std::vector<double>&, const std::vector<double>&) -> std::vector<double> {
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
    EXPECT_EQ(std::is_trivially_destructible_v<QuantityType>, std::is_trivially_destructible_v<TypeParam>);
    EXPECT_EQ(std::is_copy_constructible_v<QuantityType>, std::is_copy_constructible_v<TypeParam>);
    EXPECT_EQ(std::is_nothrow_copy_constructible_v<QuantityType>, std::is_nothrow_copy_constructible_v<TypeParam>);
    EXPECT_EQ(std::is_move_constructible_v<QuantityType>, std::is_move_constructible_v<TypeParam>);
    EXPECT_EQ(std::is_nothrow_move_constructible_v<QuantityType>, std::is_nothrow_move_constructible_v<TypeParam>);
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

    EXPECT_EQ((std::is_nothrow_constructible_v<QuantityType2, const TypeParam&>),
              (std::is_nothrow_constructible_v<TypeParam, const TypeParam&>) );

    EXPECT_EQ((std::is_nothrow_constructible_v<QuantityType2, TypeParam&&>),
              (std::is_nothrow_constructible_v<TypeParam, TypeParam&&>) );
}

TYPED_TEST(TestQuantityFixture, TestInitializerListConstructor) {
    using MeterVector = BasicQuantity<std::vector<double>, MeterUnit>;

    MeterVector q(std::in_place, {1.0, 2.0, 3.0, 4.0});
    EXPECT_EQ(q.magnitude(), (std::vector{1.0, 2.0, 3.0, 4.0}));
    EXPECT_EQ(q.units(), MeterUnit);
}

template <typename T> class TestQuantityIncompatbility : public ::testing::Test {};

using QuantityTypes = ::testing::Types<std::pair<Mole, Ampere>, std::pair<Mole, Meter>, std::pair<Mole, Candela>,
                                       std::pair<Mole, Gram>, std::pair<Mole, Kelvin>, std::pair<Mole, Second>,
                                       std::pair<Mole, Radian>, std::pair<Hertz, Becquerel>>;
TYPED_TEST_SUITE(TestQuantityIncompatbility, QuantityTypes);

template <typename L, typename R>
concept AddableWith = requires(L l, R r) { l + r; };

template <typename L, typename R>
concept SubtractableWith = requires(L l, R r) { l - r; };

TYPED_TEST(TestQuantityIncompatbility, TestIncompatbility) {
    using LHSType = TypeParam::first_type;
    using RHSType = TypeParam::second_type;

    EXPECT_FALSE((std::constructible_from<LHSType, const RHSType&>) );
    EXPECT_FALSE((std::constructible_from<LHSType, RHSType&&>) );
    EXPECT_FALSE((std::constructible_from<RHSType, const LHSType&>) );
    EXPECT_FALSE((std::constructible_from<RHSType, LHSType&&>) );
    EXPECT_FALSE((AddableWith<LHSType, RHSType>) );
    EXPECT_FALSE((AddableWith<RHSType, LHSType>) );
    EXPECT_FALSE((SubtractableWith<LHSType, RHSType>) );
    EXPECT_FALSE((SubtractableWith<RHSType, LHSType>) );
}

template <typename T> class TestQuantityConversionsPrefixes : public ::testing::Test {};

using QuantitTypePrefixes =
    ::testing::Types<std::pair<Mole, KiloMole>, std::pair<Ampere, KiloAmpere>, std::pair<Meter, KiloMeter>,
                     std::pair<Candela, KiloCandela>, std::pair<Gram, KiloGram>, std::pair<Second, KiloSecond>,
                     std::pair<Kelvin, KiloKelvin>, std::pair<Radian, KiloRadian>>;
TYPED_TEST_SUITE(TestQuantityConversionsPrefixes, QuantitTypePrefixes);

TYPED_TEST(TestQuantityConversionsPrefixes, TestConversionPrefix) {
    using LHSType = TypeParam::first_type;
    using RHSType = TypeParam::second_type;

    LHSType q1{RHSType{1.0}};
    EXPECT_EQ(q1.magnitude(), 1'000);

    RHSType q2{LHSType{1.0}};
    EXPECT_FLOAT_EQ(q2.magnitude(), 1e-3);

    LHSType q3 = RHSType{1.0};
    EXPECT_EQ(q3.magnitude(), 1'000);

    RHSType q4 = LHSType{1.0};
    EXPECT_EQ(q4.magnitude(), 1e-3);
}

TEST(TestQuantityConversionsScale, TestAngle) {
    const Radian r{std::numbers::pi};
    const Degree d{180.0};

    const Degree d2{r};
    const Radian r2{d};

    EXPECT_FLOAT_EQ(d2.magnitude(), 180.0);
    EXPECT_FLOAT_EQ(r2.magnitude(), std::numbers::pi);
}

TEST(TestQuantityConversionsScale, TestLength) {
    const Foot f{1};
    const Inch i{12};

    const Foot f2{i};
    const Inch i2{f};

    EXPECT_FLOAT_EQ(f2.magnitude(), 1);
    EXPECT_FLOAT_EQ(i2.magnitude(), 12);
}

TEST(TestQuantityConversionsScale, TestMass) {
    const PoundMass m{2.20462};
    const KiloGram kg{1};

    const PoundMass m2{kg};
    const KiloGram kg2{m};

    EXPECT_FLOAT_EQ(m2.magnitude(), 2.20462);
    EXPECT_FLOAT_EQ(kg2.magnitude(), 1);
}

template <typename T> class TestQuantityArithmeticValidity : public ::testing::Test {};

using QuantityArithmeticTypes =
    ::testing::Types<std::pair<Mole, KiloMole>, std::pair<Ampere, KiloAmpere>, std::pair<Meter, KiloMeter>,
                     std::pair<Candela, KiloCandela>, std::pair<Gram, KiloGram>, std::pair<Second, KiloSecond>,
                     std::pair<Kelvin, KiloKelvin>, std::pair<Radian, KiloRadian>, std::pair<Foot, Inch>,
                     std::pair<Foot, Meter>, std::pair<Degree, Radian>, std::pair<KiloGram, PoundMass>>;
TYPED_TEST_SUITE(TestQuantityArithmeticValidity, QuantityArithmeticTypes);

TYPED_TEST(TestQuantityArithmeticValidity, TestAddition) {
    using LHSType = TypeParam::first_type;
    using RHSType = TypeParam::second_type;

    EXPECT_TRUE((AddableWith<LHSType, RHSType>) );
    EXPECT_TRUE((AddableWith<RHSType, LHSType>) );
}

TYPED_TEST(TestQuantityArithmeticValidity, TestSubtraction) {
    using LHSType = TypeParam::first_type;
    using RHSType = TypeParam::second_type;

    EXPECT_TRUE((SubtractableWith<LHSType, RHSType>) );
    EXPECT_TRUE((SubtractableWith<RHSType, LHSType>) );
}

TYPED_TEST(TestQuantityIncompatbility, TestAddition) {
    using LHSType = TypeParam::first_type;
    using RHSType = TypeParam::second_type;

    EXPECT_FALSE((AddableWith<LHSType, RHSType>) );
    EXPECT_FALSE((AddableWith<RHSType, LHSType>) );
}

TYPED_TEST(TestQuantityIncompatbility, TestSubtraction) {
    using LHSType = TypeParam::first_type;
    using RHSType = TypeParam::second_type;

    EXPECT_FALSE((SubtractableWith<LHSType, RHSType>) );
    EXPECT_FALSE((SubtractableWith<RHSType, LHSType>) );
}
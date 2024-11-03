#include "Quantity.hpp"
#include "Unit.hpp"

#include <chrono>
#include <gtest/gtest.h>

using namespace Maxwell;

struct F
{
};

TEST(Foo, F)
{
    [[maybe_unused]] Maxwell::BasicQuantity<int, Maxwell::meterUnit> f{};
    [[maybe_unused]] Maxwell::BasicQuantity<F, Maxwell::meterUnit>   q{};

    [[maybe_unused]] Maxwell::BasicQuantity q3(std::chrono::seconds(1));
}
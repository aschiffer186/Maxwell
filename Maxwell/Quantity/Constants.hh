/**
 * @file Constants.hh
 * @author Alex Schiffer
 * @brief
 * @version 0.1
 * @date 2024-03-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include "QuantityBase.hh"
#include "QuantityLiterals.hh"

using namespace Maxwell::QuantityLiterals;

namespace Maxwell::Constants
{
    inline constexpr auto c = 299'792'458_m / 1_s;
} // namespace Maxwell::Constants

#endif
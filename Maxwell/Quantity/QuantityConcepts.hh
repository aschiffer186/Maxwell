/**
 * @file QuantityConcepts.hh
 * @author Alex Schiffer
 * @brief
 * @version 0.1
 * @date 2024-02-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef QUANTITY_CONCEPTS_HH
#define QUANTITY_CONCEPTS_HH

#include <concepts>
#include <type_traits>

namespace Maxwell
{
    namespace _detail
    {
        template <typename Tp>
        concept _integer =
            std::integral<Tp> && !std::same_as<std::remove_cv_t<Tp>, bool> &&
            !std::same_as<std::remove_cv_t<Tp>, char> &&
            !std::same_as<std::remove_cv_t<Tp>, unsigned char> &&
            !std::same_as<std::remove_cv_t<Tp>, signed char>;

        template <typename Tp>
        concept _number = std::floating_point<Tp> || _integer<Tp>;

        template <typename Tp>
        concept _basicArithmetic = std::movable<Tp> && (!_number<Tp>) &&
                                   requires(Tp a, Tp b, double d) {
                                       a + b;
                                       a - b;
                                       a *b;
                                       a / b;
                                       d *a;
                                   };
    } // namespace _detail

    /**
     * @brief Arithmetic concept
     *
     * Concept specifying the requirements of Arithmetic types.
     * An Arithmetic type is either a built-in integral type that is
     * not a char or bool, a built-in floating-point type, or a movable,
     * non-fundamental type that supports the arithmetic operations and
     * can be mutiplier by double.
     *
     */
    template <typename Tp>
    concept Arithmetic = _detail::_basicArithmetic<Tp> || _detail::_number<Tp>;

    /**
     * @brief NothrowArithmetic concept
     *
     * Concept specifying the requirements of NothrowArithmetic types.
     * A NothrowArithmetic type meets all the requirements of an Arithmetic type
     * and its arithmetic operations are marked noexcept
     *
     */
    template <typename Tp>
    concept NothrowArithmetic =
        Arithmetic<Tp> && requires(Tp a, Tp b, double d) {
            noexcept(a + b);
            noexcept(a - b);
            noexcept(a * b);
            noexcept(a / b);
            noexcept(d * a);
        };
} // namespace Maxwell

#endif
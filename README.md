# Introduction
Maxwell is a strongly typed units library for C++ 20. It enables checking of units at compile-time 
as well as conversion of units at compile-time. This ensures that if your program compiles, your dimensional analysis is correct and all unit conversions have automatically been performed. Because all of the conversions are calculated at compile-time, there is very little run-time overhead to using this library.

![license](https://img.shields.io/badge/license-MIT-orange.svg) ![language](https://img.shields.io/badge/language-c++-blue.svg) ![c++](https://img.shields.io/badge/std-c++20-blue.svg)

# Getting Started
The most basic way to use Maxwell is to use it as a header only library. Simple add `Maxwell.hpp` to your project. 

The most commonly used type in the library is the `Quantity` class template. This essentially represents a `double` with an extra units tag, allowing for verifying the units of all expressions.

The `Quantity` class template can (and should) be used anywhere a `double` can be used!

## Using Type Deduction
Generally, using `auto` as the result of the operations on `BasicQuantities` will have unexpected results. It means you accepet the result of the calculation as is. Addition and subtrtaction of units returns a `BasicQuantity` whose units are the same as th left hand side of the addition or subtraction Multiplication and division of units always returns a `BasicQuantity` whose units are SI base units. It is better to specify the units of the quantity you expect explicitly.

```c++
auto q1 = 1.0_m + 2.0_ft // Result is in m
auto q2 = 2.0_ft + 1.0_m // Result is in ft

auto q4 = 1.0_ft * 2.0_ft // Result is in m^2
auto q3 = 1.0_mi / 3.0_hr // Result is in m/s
```

# Integration with C++ Standard Library. 
The `BasicQuantity` type behaves well with types in the standard library.
## Container Support
`BasicQuantity` specializes `std::hash` whenever its magnitude type specializes `std::hash`, allowing it to be used in unordered containers (e.g. `std::unordered_map` and `std::unordered_set`). It provides an `operator <` when its magnitude type provides `operator <` allowing it to be used with ordered containers (e.g. `std::map` and `std::set`)

## Formatting Support
`BasicQuantity` specializes `std::formatter`, allowing it to be used in `std::format` and `std::print` (C++ 23 and later). It also provides an overloaded `operator <<` for easy printing to output streams.

## Integration with C++ Math Library
Maxwell provides type-safe wrappers around most functions in the `<cmath>` header, elimining the need 
to step outside of the type safety provided by Maxwell. These functions also follow the `constexpr` supported provided in C++23 and C++26.

## Integration with Chrono Library
`BasicQuantity` can be constructed from and converted to an instance of `std::chrono::duration` when the `BasicQuantity` has units of time. This construction and conversion is implict when no information is loss. 

Class template argument deduction is available when constructing from a `std::chrono::duration` when the period of the duration matches a unit in Maxwell.

# `BasicQuantity` Type 
Class template `BasicQuantity` is the fundamental type in Maxwell; it represents a quantity that has a magnitude and units. The type of the magnitude can be any C++ type that isn't `cv`-qualified. For convenience, the type aliases `Quantity` and `IQuantity` are provided
* `Quantity` is an alias of `BasicQuantity` whose magnitude type is `double`
* `IQuantity` is an alias of `BasicQuantity` whose magnitude type is `int`

Athough the `BasicQuantity` template will most commonly be instantiated with a `double`, there is nothing stopping you from using other types, e.g. `std::vector<int>`
```c++
// Creates a quantity that is a vector of meters
Maxwell::BasicQuantity<std::vector<double>, Maxwell::meterUnit> q(std::in_place, {1.0, 2.0, 3.0}); 
```
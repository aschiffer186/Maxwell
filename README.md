# Introduction
Maxwell is a strongly typed units library for C++ 20. It enables checking of units at compile-time 
as well as conversion of units at compile-time. This ensures that if your program compiles, your dimensional analysis is correct and all unit conversions have automatically been performed. Because all of the conversions are calculated at compile-time, there is very little run-time overhead to using this library.

![license](https://img.shields.io/badge/license-MIT-orange.svg) ![language](https://img.shields.io/badge/language-c++-blue.svg) ![c++](https://img.shields.io/badge/std-c++20-blue.svg)

# Getting Started
The most basic way to use Maxwell is to use it as a header only library. Simple add `Maxwell.hpp` to your project. 

The most commonly used type in the library is the `Quantity` class template. This essentially represents a `double` with an extra units tag, allowing for verifying the units of all expressions.

The `Quantity` class template can (and should) be used anywhere a `double` can be used!

## `BasicQuantity` Type 
Class template `BasicQuantity` is the fundamental type in Maxwell; it represents a quantity that has a magnitude and units. The type of the magnitude can be any C++ type that isn't `cv`-qualified. For convenience, the type aliases `Quantity` and `IQuantity` are provided
* `Quantity` is an alias of `BasicQuantity` whose magnitude type is `double`
* `IQuantity` is an alias of `BasicQuantity` whose magnitude type is `int`
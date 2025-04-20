# Maxwell
A compile-time dimensional analysis library for C++20 with no dependencies

# Contents

- [Maxwell](#maxwell)
- [Contents](#contents)
- [Overview](#overview)
- [Getting Started](#getting-started)

# Overview
Maxwell is a strongly-typed, header-only, dimensional analysis library for C++20. It performs dimensional analysis, including verification of dimensional coherence and calculation of unit conversion factors, at compile-time with no run-time overhead.

The library consists of one main header file, [Maxwell.hpp](include/Maxwell.hpp) and a library of unit tests. Additionally, several examples showing how to use Maxwell are provided. 

# Getting Started
To get started using Maxwell, simply include the header file `Maxwell.hpp` to your project. For convenience, you may wish to add the using director `using namespace maxwell;` to your project as well. To use the pre-defined unit literals, you must also add the using directives:
* `using namespace maxwell::metric_literals;` for literals for metric units

Simple example: 
```c++
#include <iostream>

#include <Maxwell.hpp>

using namespace maxwell;
using namespace maxwell::metric_literals; // For accessing literals
```

The main type of the Maxwell library is the `quantity` type. This type represents a dimensioned quantity with both a magnitude and units! Unlike other units libraries, Maxwell places no restrictions and relies on minimal assumptions about the underlying type of the quantities used. However, the default type used by the quantity is `double`.
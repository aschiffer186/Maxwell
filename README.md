# Overivew 
Maxwell is a header only, strongly-typed units library for C++ 20. It performs unit checking and conversions at compile-time; if a program is well-formed then all of its units are consistent. It also performs automatic units conversions at compile-time, eliminating common errors such as converting between degrees and radians. Since the conversions are calculated at compile-time, there is minimal 
runtime overhead from using the library.

A basic example is shown below
```c++
#include <Maxwell.hpp>

using namespace Maxwell;

int main()
{
    Foot ft{1.0}
    Inch in{ft};

    std::cout << in << "\n; //Prints "12 in"

    //PoundMass lbm{ft}; Compiler error!
    return 0;
}
```

The library consists of a heder file, Maxwell.hpp, and unit tests for the header file.

# Getting Started
Most functionality can be accessed by adding `Maxwell.hpp` to your project. All types live within the Maxwell namespace. To make acces more convenient, you can add a using declaration.

```c++
#include <Maxwell.hpp>
 
using namespace Maxwell;
```

The central type of Maxwell is the class template `BasicQuantity`. This type can be thought of a wrapper around any type that supports the operarors +, -, *, / and can be multiplied by a `double` with an additional tag for the unit.. For the common case where the typed wrapped is a double the type alias template `Quantity` is provided. Addtionally, there are many type aliases of `Quantity` for common units. These type aliases are the name of the units of the quantity (e.g. `Meter` represents a meter).

The type `BasicQuantity` can be used anywhere the underlying type can be used. For elementary types, literal suffixes are predfined by the library. For non elementary types, the quantites must be constructed expclitily

```c++
const double area = 15 * 5 + 10 * 10; // 175 m^2??
const SqMeter area = 15_m2 * 5_m2 + 10_m2 + 10_m2; // 175 m^2

// Assumes the appropriate operators have been provided somehwere for two vectors
const std::vector<double> areas = std::vector{15, 15} * std::vector{5, 5} + 
    std::vector{10, 10} * std::vector{10, 10}; // {175 m^2, 175 m^2}??
using SqMeterVector = BasicQuantity<std::vector<double>, SqMeterUnit>;

const SqMeterVector area = SqMeterVector{15, 15} * SqMeterVector{5, 5} + 
    SqMeterVector{15, 15} * SqMeterVector{5, 5} // {175 m^2, 175 m^2}
```

The major benefit to using quantities is that unit conversions happen implicitly. Conversions are checked at compile-time, so errors causea compiler error rather than a run-time error.

```c++
const double length_cm = 200;
const double length_m = 15;
const double area_m = length_cm * lengt_m; // Uh-oh, no conversion

const SqMeter area_m = 200_cm * 15_m // Automatically converted to 15 m^2

const double mass_kg = 15;
const double meangingless = mass_kg + length_m; // Yikes 
const SqMeter error = 15_m + 15_kg; // Won't compile
```

For complicated expressions, `auto` can be used. However, be carefule using auto. Arithmetic expressions always convert their output to SI base units
```c++
const auto area = 200_cm * 100_cm // the value of area is 2 m^2, intended? maybe no.
```
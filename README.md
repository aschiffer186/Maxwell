# Overview
Maxwell is a strongly typed units library for C++20 and later standards based on the ISQ 80000 units specification. 
It provides compile-time checking of and conversion between units. 

This README focuses on how to use Maxwell. For a more detailed description, build the Doxygen target.
# Installation 
The easiest way to install Maxwell is by cloning the repo and using the CMake install command. 
Once you have cloned the repo, run the following commands inside the folder where you cloned the repo
```
mkdir build
cd build
cmake .. 
cmake --build . --config Release --target install
```

Maxwell can then be found using `find_package`: 
```
find_package(Maxwell CONFIG REQUIRED)
```

The following variables are provided: 
* `Maxwell_DIR`: The location where the Maxwell CMake was installed.
* `Maxwell_INCLUDE_DIR`: The location where the Maxwell header files were installed.

To use Maxwell in your CMake project, simply add `Maxwell_INCUDE_DIR` to your project's include directories and link against the `Maxwell::Maxwell` target provided.

## Building and Running Tests
To build and run units, pass the `BUILD_TESTS` flag to the CMake configuration step. 
Then run `ctest`. 
After changing to your build folder, run the following commands
```
cmake -DBUILD_TESTS=On ..
cmake --build . --config Debug
ctest -C debug .
```

## Building Documentation
Building documentation can be down using the "build-docs" workflow. Note that doxygen and sphinx are required to build the documentation. 
Doxygen can be installed using your OS's package manager.
Sphinx and other requirements can be installed using `pip` and the requirements.txt file inside the `doc` folder.
From the main Maxwell directory run the following command: 
```
cmake --workflow --preset build-docs
```
This should build all documentation and place it in the `docs` folder of your build directory. Doxygen documentation will be placed into `docs/doxygen`.

# Getting Started 
The simplest way to get started with Maxwell is the include the `Maxwell.hpp` header in your source code. 
This header provides access to all types in Maxwell. 
For convenience, you may wish to add a using declaration for Maxwell as well. 

Example: 
```c++
#include <Maxwell/Maxwell.hpp>

using namespace maxwell; 

int main()
{
    si::meter<> m1{100};
    std::cout << m1 << "\n"; // prints 100 m; 
    return 0;
}
```

# Quantities, Units, and Quantity Values
Maxwell is designed to follow the ISO-800001 standard. There are two concetps central to the standard: quantites and quantity values. 

## Quantities
A *quantity* provides a quantitative description of a phenomena, substance or property. Quantities can be categorized into different groups; the group a category belongs to is called the *kind* of a quantity. Quantities can only be added to, subtracted from, and assigned to quantities of the same kind.

Quantities are orgaized into systems of quantities. In a system of quantities, there are *base quantities* and *derived quantities* which are expressed in terms of equations involving base quantities. The equation relating a derived quantity to the base quantity, discarding any numerical constants, is known as the *dimensions* of the quantity. Note that two quantities can belong to different kinds even if they have the same dimesions.

In Maxwell, quantities are represented by the `quantity` class template. For mose uses of Maxwell, the details of the `quantity` class are unimportant. At a high level, the `quantity` class keeps track at compile-time the kind of a quantity and verifies operations are only performed on quantities of the same kind.

## Units 
A *unit* is a particular example of a quantity chose as a reference unit. Any quantity can be expressed as a product of a number, called the numerical value of the quantity, and the unit. 

Like quantities, units can be organized into a system of units. Each system of units has *base units* that correspond to base quantites and *derived units* formed from those units. 

In Maxwell, quantities are represented by the `unit` class template. Like the `quantity` class template, the details of the `unit` class template are unimportant for most uses. At a high level, the `unit` class keeps track of the quantity it belongs to and the relationship between the unit and its base units. This allows for compile-time conversion between units in Maxwell.

## Quantity Values
A quantity value is the product of a numerical value and a unit. Quantity values can only be assigned to and compared with quantity values of the same kind; addition and subtraction can also only be performed on quantities of the same kind. Multiplying or dividing two quantity values provides a new quantity value of a different kind. 

In Maxwell, quantity values are represented by the `quantity_value` class template. This is the most commonly used type in Maxwell.

# `quantity_value` class 
## Overview
The `quantity_value` class is the central type used in Maxwell. 
```c++
template<auto Unit, auto Quantity = Unit.quantity, typename UnderlyingType = double>
class quantity_value;
```
The template parameters of the `quantity_value` specify the unit the numerical value of the quantity is defined in reference to, the kind of quantity represented by the quantity value, and the type of the numerical value stored in the quantity. The quantity value class is desgined to be used wherever the underlying type can be used. 

```c++
int main()
{
    using namespace maxwell; 
    quantity_value<si::meter_unit> q1{100.0}; // A length quantity exprressed in meters whose underlying type is double 

    quantity_value<nano<si::meter_unit>, wavelength, float> q2{200.0}; // A wavelength quantity expressed in nanometers whos underlying type is float
}
```
In most cases, it is not necessary to specify the `Quantity` or `UnderlyingType` template parameters. The `Quantity` template parameter only needs to be specified if a custom derived quantity needs to be used. The `UnderlyingType` parameter needs to be specified whenever the underlying type is not `double`.

> [!WARNING]
> If an integral type is specified for the underlying type, integer division will be performed when dividing quantity values and truncation will be performed when converting from another quantity value.

An instance of `quantity_value` can only be assigned to `quantity_values` of the same kind or a more general kind. If this is violated, a compile-time error is issued. Therefore, if a program using Maxwell is well formed, it is guaranteed the units of the expressions in the program are correct. 
```c++
quantity_value<si::meter_unit> q1 = quantity_value<si::ampere_unit>{}; // Error - will not compile because units are different
// Assume wavelength is a derived quantity from length
quantity_value<si::meter_unit, wavelength> = quantity<si::meter_unit>{}; // Error - will not compile because quantity kinds are different
```

## Constructing Quantity Values
Quantity values can always be constructed from the underlying type. The value passed to the constructor is forward to the underlying value for efficiency.
```c++
quantity_value<si::meter_unit> q1{100.0}; // q1 represents a length of 100 m 
quantity_value<si::meter_unit, wavelength> q2{500.0}; // q3 represents a length of 500 m 
quantity_value<si::meter_unit, isq::length_quantity, BigNumber> q3{BigNumber{factorial(500)}}; // Big number is moved into quantity. 
```

A quantity value can be constructed from any quantity of the same kind, even if the units are different. In this case, the units of the other quantity are automatically converted to the units of the quantity being constructed. Importantly, the conversion factor is computed **at compile time** instead of at **runtime**. This means there is virtually no overhead using Maxwell instead of raw arithmetic types. 
```c++
quantity_value<si::meter_unit> q1 = quantity_value<si::kilometer_unit>{1}; // q1 represents a length of 1,000 m
quantity_value<us::lb_unit> q2 = quantity_value<si::kilogram_unit>{2}; // q2 represens a mass of 4.40925 pounds
```

For convenience, many type aliases are provided to make constructing quantites less verbose. If no underlying type is specified, `double` is assumed. These aliases are provided in the following namespaces: 
* `maxwell::si` - SI units 
* `maxwell::us` - US customary units
```c++
si::meter<> q1{100}; // Same as quantity_value<si::meter_unit, isq::length_quantity, double> q1{100};
si::mole<long double> q2{25}; // Same as quantity_value<si::meter_unit, isq::amount_quantity, long double> q2{25}
```

> [!IMPORTANT]
> The angle brackets (<>) are always necessary even when the default type is used. This is due to how C++ type alias and variable templates are defined.
 
For larger types, the underlying value of the quantity can be constructed in place, similar to `std::optional` or `std::variant`. 
```c++
using vector_quantity = quantity_value<si::meter_unit, isq::length_quantity, std::vector<double>>;
vector_quantity q1(std::in_place, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}, MyCustomAllocator{}); // Constructs the underlying value in place!
```

Metric prefixes are provided as type alias templates in the main `Maxwell` namespace. This avoids the need to define prefixes for every single unit. The exception is kilogram which has a dedicated type. 

```c++
maxwell::nano<maxwell::si::second<>> ns{100}; // 100 ns 
maxwell::kilo<maxwell::si::meter<>> km{100}; // 100 km
```

Type aliases are provided for all metric prefixes including quetta, ronna, ronta, and quecto.

For more complicated units or more complicated initialization expressions, abbreviated symbols are provided to ease construction.
```c++
si::newton_meter<> q1 = 100 * N * m; // q1 represents 100 N-m.
```

The definition of the `quantity` prevents mixing incompatible quantities with the same dimenions, but allows for initializing them with exprerssions consisting of the base units. 
```c++
si::newton_meter<> q1 = 100 * N * m; // OK - q1 represents 100 N-m
si::joule<> q2 = q1; // Error - will not compile
auto q3 = 100.0 * kg * s / (A * C); // q3 represents 100.0 kg * s * A^-1 * C^-1
```

> [!WARNING]
> When using symbols, if an integer literal is used with the `auto` keyword, the underlying quantity will have an integral type.

### Constructing from Standard Library Types
Instancesf of `quantity_value` that represent time can be implicitly converted from instances of `std::chrono::duration`. 
```c++
using namespace std::chrono_literals;
maxwell::milli<maxwell::si::second<>> q{std::chrono::seconds(1)}; // Ok - 1000 millisecond
maxwell::si::hour<> q2 = 1_m; // Ok - 1/60 hour
```

## Operations on Quantity Values
All instances of `quantity_value` implement all build-in arithmetic operations (+, -, *, /, %) if the underlying type supports the arithmetic operations. 

Multiplication and division of two instances of `quantity_value` creates a new quantity value whose units are the product or quotient of the two input instances of `quantity_value`. 

```c++
maxwell::si::meter<> m1{100.0}
maxwell::si::meter<> m2{100.0};

maxwell::si::square_kilometer<> area = m1 * m2; // 0.01 square kilometers 

maxwell::si::meter<> m3 = area / m1; // 100 meters
```

Addition and subtraction can ony be performed on instances of `quantity_value` representing the same quantity and that have the same reference point. 

```c++
maxwell::centi<maxwell::si::meter<>> cm = maxwell::si::meter<>{1} + maxwell::si::meter<2>{1}; // 200 cm 
maxwell::si::ampere<> A = cm + maxwell::si::ampere<>{2}; // Error - will not compile 

using wavelength = quantity_value<si::meter, wavelength_quantity>;
maxwell::si::meter<> w = maxwell::si::meter<>{1} + wavelength{2}; // Error - will not compile
```

> [!NOTE]
> Arithmetic operations will result in types that are isomorphic, but not necessarily the same, as predefined units. 
> ```c++
> si::meter_unit * si::meter_unit == si::square_meter_unit; // Guaranteed to be true 
> std::is_same_v<si::meter_unit * si::meter_unit, si::square_meter_unit>; // Not guaranteed to be tree
> ```

> [!NOTE]
> Using `auto` with arithmetic operations may result in unexpected values. 
> ```c++
> auto q1 = maxwell::si::meter<>{100.0} * maxwell::si::kilometer<10.0>; // 1,000 m*km 
> auto q2 = maxwell::us::foot<>{1} + maxwell::us::inch<>{1}; // 13/12 ft.

# `quantity_holder` Class Template
Maxwell also provides the `quantity_holder` class template to specify units at run-time rather than compile-time. 
```c++
template<auto Quantity, typename T>
class quantity_holder;
```

Compile-time verification that operations on instances of `quantity_holder` is still performed, but unit conversions will be perfomed at run-time instead of compile-time. V
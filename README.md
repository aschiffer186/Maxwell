# Overview
Maxwell is a strongly typed units library for C++20 and later standards based on the ISQ 80000 units specification. It provides compile-time checking of and conversion between units. 

# Installation 
TBD


# Getting Started 
The simplest way to get started with Maxwell is the include the `Maxwell.hpp` header in your source code. This header provides access to all types in Maxwell. For convenience, you may wish to add a using declaration for Maxwell as well. 

Example: 
```c++
#include <Maxwell.hpp>

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

Users Guide
===========

Getting Started
---------------

Maxwell provides strongly-typed units representations that are designed to be used wherever the underlying type can be used.

Using Header Files 
^^^^^^^^^^^^^^^^^^
The simplest way to get started with Maxwell is the include the `Maxwell.hpp` header in your source code. 
This header provides access to all types in Maxwell. 
For convenience, you may wish to add a using declaration for Maxwell as well. 

Example: 

.. code-block:: c++

    #include <Maxwell/Maxwell.hpp>

    using namespace maxwell; 

    int main()
    {
        si::meter<> m1{100};
        std::cout << m1 << "\n"; // prints 100 m; 
        return 0;
    }

Using Modules
^^^^^^^^^^^^^
TBD


Quantities, Units, and Quantity Values
--------------------------------------
Maxwell is designed to follow the ISO-800001 standard. There are two concetps central to the standard: quantites and quantity values. 

Quantities
^^^^^^^^^^
A *quantity* provides a quantitative description of a phenomena, substance or property. 
Quantities can be categorized into different groups; the group a category belongs to is called the *kind* of a quantity. 
Quantities can only be added to, subtracted from, and assigned to quantities of the same kind.

Quantities are orgaized into systems of quantities. In a system of quantities, there are *base quantities* and *derived quantities* which are expressed in terms of equations involving base quantities. 
The equation relating a derived quantity to the base quantity, discarding any numerical constants, is known as the *dimensions* of the quantity. 
Note that two quantities can belong to different kinds even if they have the same dimesions.

In Maxwell, quantities are represented by the :code:`quantity` class template. 
For most uses of Maxwell, the details of the :code:`quantity` class are unimportant. 
At a high level, the :code:`quantity` class keeps track at compile-time the kind of a quantity and verifies operations are only performed on quantities of the same kind.

Units 
^^^^^
A *unit* is a particular example of a quantity chose as a reference unit. 
Any quantity can be expressed as a product of a number, called the numerical value of the quantity, and the unit. 

Like quantities, units can be organized into a system of units. Each system of units has *base units* that correspond to base quantites and *derived units* formed from those units. 

In Maxwell, quantities are represented by the :code:`unit` class template. 
Like the :code:`quantity` class template, the details of the :code:`unit` class template are unimportant for most uses. 
At a high level, the :code:`unit` class keeps track of the quantity it belongs to and the relationship between the unit and its base units. 
This allows for compile-time conversion between units in Maxwell.

Quantity Values
^^^^^^^^^^^^^^^
A quantity value is the product of a numerical value and a unit. 
Quantity values can only be assigned to and compared with quantity values of the same kind; addition and subtraction can also only be performed on quantities of the same kind. 
Multiplying or dividing two quantity values provides a new quantity value of a different kind. 

In Maxwell, quantity values are represented by the :code:`quantity_value` class template. This is the most commonly used type in Maxwell.

:code:`quantity_value` class 
----------------------------
Overview
^^^^^^^^
The :code:`quantity_value` class is the central type used in Maxwell.

.. code-block:: c++

    template<auto Unit, auto Quantity = Unit.quantity, typename UnderlyingType = double>
    class quantity_value;

The template parameters of the :code:`quantity_value` specify the unit the numerical value of the quantity is defined in reference to, the kind of quantity represented by the quantity value, and the type of the numerical value stored in the quantity. 
The quantity value class is desgined to be used wherever the underlying type can be used.

.. code-block:: c++

    int main()
    {
        using namespace maxwell; 
        quantity_value<si::meter_unit> q1{100.0}; // A length quantity exprressed in meters whose underlying type is double 
        quantity_value<nano<si::meter_unit>, wavelength, float> q2{200.0}; // A wavelength quantity expressed in nanometers whos underlying type is float
    }


In most cases, it is not necessary to specify the :code:`Quantity` or :code:`UnderlyingType` template parameters. 
The :code:`Quantity` template parameter only needs to be specified if a custom derived quantity needs to be used. 
The :code:`UnderlyingType` parameter needs to be specified whenever the underlying type is not :code:`double`.

.. warning::
    If an integral type is specified for the underlying type, integer division will be performed when dividing quantity values and truncation will be performed when converting from another quantity value.

An instance of :code:`quantity_value` can only be assigned to :code:`quantity_values` of the same kind or a more general kind. If this is violated, a compile-time error is issued. 
Therefore, if a program using Maxwell is well formed, it is guaranteed the units of the expressions in the program are correct.

.. code-block:: c++

    quantity_value<si::meter_unit> q1 = quantity_value<si::ampere_unit>{}; // Error - will not compile because units are different
    // Assume wavelength is a derived quantity from length
    quantity_value<si::meter_unit, wavelength> = quantity<si::meter_unit>{}; // Error - will not compile because quantity kinds are different

Constructing Quantity Values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Quantity values can always be constructed from the underlying type. 
The value passed to the constructor is forward to the underlying value for efficiency.

.. code-block:: c++

    quantity_value<si::meter_unit> q1{100.0}; // q1 represents a length of 100 m 
    quantity_value<si::meter_unit, wavelength> q2{500.0}; // q3 represents a length of 500 m 
    quantity_value<si::meter_unit, isq::length_quantity, BigNumber> q3{BigNumber{factorial(500)}}; // Big number is moved into quantity. 

A quantity value can be constructed from any quantity of the same kind, even if the units are different. 
In this case, the units of the other quantity are automatically converted to the units of the quantity being constructed. 
Importantly, the conversion factor is computed **at compile time** instead of at **runtime**. 
This means there is virtually no overhead using Maxwell instead of raw arithmetic types.

.. code-block:: c++

    quantity_value<si::meter_unit> q1 = quantity_value<si::kilometer_unit>{1}; // q1 represents a length of 1,000 m
    quantity_value<us::lb_unit> q2 = quantity_value<si::kilogram_unit>{2}; // q2 represens a mass of 4.40925 pounds

For convenience, many type aliases are provided to make constructing quantites less verbose. If no underlying type is specified, :code:`double` is assumed. 
These aliases are provided in the following namespaces: 

* :code:`maxwell::si` - SI units 
* :code:`maxwell::us` - US customary units

For a complete list, see :doc:`predefined-units <predefined_units>`

.. code-block:: c++

    si::meter<> q1{100}; // Same as quantity_value<si::meter_unit, isq::length_quantity, double> q1{100};
    si::mole<long double> q2{25}; // Same as quantity_value<si::meter_unit, isq::amount_quantity, long double> q2{25}

.. important::
    The angle brackets (:code:`<>`) are always necessary even when the default type is used. 
    This is due to how C++ type alias and variable templates are defined.
 
For larger types, the underlying value of the quantity can be constructed in place, similar to `std::optional` or `std::variant`. 

.. code-block:: c++

    using vector_quantity = quantity_value<si::meter_unit, isq::length_quantity, std::vector<double>>;
    vector_quantity q1(std::in_place, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}, MyCustomAllocator{}); // Constructs the underlying value in place!

Metric prefixes are provided as type alias templates in the main :code:`Maxwell` namespace. 
This avoids the need to define prefixes for every single unit. The exception is kilogram which has a dedicated type. 

.. code-block:: c++

    maxwell::nano<maxwell::si::second<>> ns{100}; // 100 ns
    maxwell::kilo<maxwell::si::meter<>> km{100}; // 100 km

Type aliases are provided for all metric prefixes including quetta, ronna, ronta, and quecto.

For more complicated units or more complicated initialization expressions, abbreviated symbols are provided to ease construction.

.. code-block:: c++

    si::newton_meter<> q1 = 100 * N * m; // q1 represents 100 N-m.

The definition of the :code:`quantity` prevents mixing incompatible quantities with the same dimenions, but allows for initializing them with exprerssions consisting of the base units.

.. code-block:: c++

    si::newton_meter<> q1 = 100 * N * m; // OK - q1 represents 100 N-m
    si::joule<> q2 = q1; // Error - will not compile
    auto q3 = 100.0 * kg * s / (A * C); // q3 represents 100.0 kg * s * A^-1 * C^-1

.. warning::
    When using symbols, if an integer literal is used with the :code:`auto` keyword, the underlying quantity will have an integral type.

Constructing from Standard Library Types
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Instances of :code:`quantity_value` that represent time can be implicitly converted from instances of :code:`std::chrono::duration`.

.. code-block:: c++

    using namespace std::chrono_literals;
    maxwell::milli<maxwell::si::second<>> q{std::chrono::seconds(1)}; // Ok - 1000 millisecond
    maxwell::si::hour<> q2 = 1m; // Ok - 1/60 hour

Operations on Quantity Values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
All instances of :code:`quantity_value` implement all built-in arithmetic operations if the underlying type supports the arithmetic operations.

Multiplication and division of two instances of :code:`quantity_value` creates a new quantity value whose units are the product or quotient of the two input instances of :code:`quantity_value`.

Multiplication and division of two instances of `quantity_value` creates a new quantity value whose units are the product or quotient of the two input instances of `quantity_value`. 

.. code-block:: c++

    maxwell::si::meter<> m1{100.0};
    maxwell::si::meter<> m2{100.0};

    maxwell::si::square_kilometer<> area = m1 * m2; // 0.01 square kilometers 

    maxwell::si::meter<> m3 = area / m1; // 100 meters

Addition and subtraction can only be performed on instances of :code:`quantity_value` representing the same quantity and that have the same reference point.

.. code-block:: c++

    maxwell::centi<maxwell::si::meter<>> cm = maxwell::si::meter<>{1} + maxwell::si::meter<2>{1}; // 200 cm 
    maxwell::si::ampere<> A = cm + maxwell::si::ampere<>{2}; // Error - will not compile 

    using wavelength = quantity_value<si::meter, wavelength_quantity>;
    maxwell::si::meter<> w = maxwell::si::meter<>{1} + wavelength{2}; // Error - will not compile

.. note::

    Arithmetic operations will result in types that are isomorphic, but not necessarily the same, as predefined units. 
    
    .. code-block:: c++

        si::meter_unit * si::meter_unit == si::square_meter_unit; // Guaranteed to be true
        std::is_same_v<si::meter_unit * si::meter_unit, si::square_meter_unit>; // Not guaranteed to be tree

.. note::

    Using :code:`auto` with arithmetic operations may result in unexpected values.

    .. code-block:: c++

        auto q1 = maxwell::si::meter<>{100.0} * maxwell::si::kilometer<10.0>; // 1,000 m*km
        auto q2 = maxwell::us::foot<>{1} + maxwell::us::inch<>{1}; // 13/12 ft.

Instances of :code:`quantity_value` support all comparison operations supported by the underlying type. 
Like addition and subtraction, two instances of :code:`quantity_value` can only be compared if they represent the same quantity and 
their units have the same reference point. 

.. warning:: 

    When two instances of :code:`quantity_value` are compared for equality, exact equality is used if the underlying type is a floating-point 
    type. The standard warnings about floating-point equality apply.``

Additionally, all functions provided by the :code:`<cmath>` header are overloaded to support instances of :code:`quantity_value` where appropriate. 
Trigonometric functions can take angles of any type; they will automatically convert the angle to radians if necessary. 

.. code-block:: c++

    using namespace maxwell::math;

    double s = sin(si:degree<>(90)); // s == 1.0 
     
For inverse trigonmetic functions, functions are provided to return angles in radians and in degrees. Functions that returned angles in degrees are suffixed 
with :code:`d`. 

.. code-block:: c++

    si::radian<> a1 = asin(1.0); // a1 == pi/2 radians
    si::degree<> a2 = asind(1.0); // a2 == 90 d`egrees

Special mathematical functions, e.g. :code:`exp` or :code:`log` are only provided for dimensionless quantities. 

:code:`quantity_holder` Class Template
---------------------------------------
Maxwell also provides the :code:`quantity_holder` class template to specify units at run-time rather than compile-time.

.. code-block:: c++

    template<auto Quantity, typename T>
    class quantity_holder;

Compile-time verification that operations on instances of :code:`quantity_holder` is still performed, but unit conversions will be performed at run-time instead of compile-time.

Defining Custom Quantities and Units 
====================================

Although Maxwell provides many predefined quantities and units, it is not possible for the library to provide all quantities and units 
that may be useful to the user. 
New quantities and units can be defined using the :code:`derived_quantity`, :code:`sub_quantity` and :code:`make_derived_unit_t` type aliases. 

The :code:`derived_quantity` type alias is used to define a new quantity with dimensions not already defined in a pre-existing quantity from the base quantities 
of a system. For example, the :code:`derived_quantity` type alias can be used to define velocity as length divided by time.

The :code:`sub_quantity` type alias is used to define a new quantity that has the same dimensions as an existing quantity, but is a different kind of quantity. 
For example, the :code:`sub_quantity` type alias can be used to define height as a sub_quantity of length.

All quantities and units in Maxwell are constants, and the :code:`quantity_value` and :code:`quantity_holder` class templates expect constants. 
This makes it easy to expression new quantities and units using arithmetic expressions involving other quanties and units. 

Although it is only necessary to use the type aliases provided, creating custom types to represent new quantities and units can result 
in less verbose error messages. 

.. code-block:: c++ 

    // Make a new quantity representing wavelength
    constexpr quantity auto wavelength = maxwell::sub_quantity<isq::length, "Wavelength">{}; 

    // Making a new type 
    constexpr struct Mach_quantity : maxwell::sub_quantity<isq::number, "Mach"> {} Mach;

    // Making a more complex quantity 
    using density_quantity = maxwell::derived_quantity<isq::mass / isq::volume, "Density">;

    // Make a new units to represent the new quantities
    constexpr unit auto Mach_unit = maxwell::derived_unit<Mach_quantity, "M">{};
    constexpr struct density_unit_type : maxwell::derived_unit<density_quantity, "DensityUnit">{} density_unit;

    // Make quantity value aliases
    using Mach = maxwell::quantity_value<si::number, Mach_quantity>;
    // Density can be expressed using any metric prefix, so make it a template 
    template <auto U> 
    using density = maxwell::quantity_value<U, density_quantity>; 
    // Example instantiation: density<kilo_unit<density_unit_type>>;

The definition can be moved inline to an alias for :code:`quantity_value` if desired.

.. code-block:: c++  

    using Mach = maxwell::quantity_value<si::number, maxwell::derived_quantity<isq::number, "Mach">>;

.. important::

    New quantities are not compatible with the quantities they are created from, even if they have the same dimensions. 
    To simply alias an existing quantity, create a new constant.

    .. code-block:: c++ 

        constexpr quantity auto my_alias = isq::length; // Represent same quantity.

Notice that no macros were required to defined new quantities, units, or aliases using them!


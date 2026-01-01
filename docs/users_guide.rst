Users Guide
===========

Getting Started
---------------

Maxwell provides strongly-typed units representations that are designed to be used wherever the underlying type can be used.

Using Header Files 
^^^^^^^^^^^^^^^^^^
The simplest way to get started with Maxwell is the include the :code:`Maxwell.hpp` header in your source code. 
This header provides access to all types in Maxwell. 
For convenience, you may wish to add a using declaration for Maxwell as well. 

Example: 

.. code-block:: c++

    #include <Maxwell.hpp>

    using namespace maxwell; 

    int main()
    {
        si::meter<> m1{100};
        std::cout << m1 << "\n"; // prints 100 m; 
        return 0;
    }

Using Modules
^^^^^^^^^^^^^
Maxwell also provides a C++20 module interface. 
To use the module interface, import the :code:`maxwell` module in your source code. 

.. code-block:: c++ 

    import maxwell;

    using namespace maxwell; 

    int main() 
    {
        si::meter<> m1{100};
        std::cout << m1 << "\n"; // prints 100 m; 
        return 0;
    }


Using Maxwell Types 
^^^^^^^^^^^^^^^^^^^ 

Maxwell provides two main types: :code:`quantity_value` (providing compile-time unit conversions) and :code:`quantity_holder` (providing run-time unit conversions). 
The easiest way to use the library is to think of these types of wrapping an underlying numeric value. 
The types can and should be used anywhere the underlying numeric type can be used. 
  
.. code-block:: c++ 

    using namespace maxwell::si::symbols;

    const double area                = 5 * 5 + 10 * 10; 
    const si::square_meter<> area1   = 5 * m * 5 * m + 10 * m * 10 * m; 
    const isq::area_holder<> area2 = 5 * m * 5 * m + 10 * m * 10 * m;

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

In Maxwell, quantity values are represented by the :code:`quantity_value` and :code:`quantity_holder` class templates. 
These are the most commonly used types in Maxwell.

Overview
--------
Maxwell supports two modes: units specified at compile-time and units specified at run-time. 

In compile-time mode, the units must be specified at compile-time as template parameters. 
This allows for unit conversions to be performed at compile-time with no additional run-time overhead. 
The :code:`quantity_value` class template is the central type used in compile-time mode.

The :code:`quantity_holder` class template is used for run-time mode. 
In run-time mode, the unit is specified at run-time as a constructor parameter.
This allows for greater flexibility in specifying units, but conversions between units incur a small run-time overhead.

In both cases, all operations are checked at compile-time to ensure that they are only performed on compatible quantity values (with one exception described below for run-time mode)

Compile-Time Mode 
-----------------

Introduction 
^^^^^^^^^^^^

As mentioned above the :code:`quantity_value` class template is used for compile-time mode.
It has the following declaration: 

.. code-block:: c++

    template <auto Units, auto Quantity = Units.quantity, typename T = double>
    class quantity_value;

In this declaration, :code:`Units` is an object that represents the units of the quantity value. 
:code:`Quantity` is an object that represents the kind of quantity the quantity value belongs to. 

Only the :code:`Units` template parameter is required; the :code:`Quantity` template parameter defaults to the quantity associated with the specified units. 
However, the :code:`Quantity` template parameter can be used to create custom strongly-type derived quantities to more naturally represent the problem domain as described below. 

Using non-type template parameters for the units and quantity values allows for a more natural syntax when working with instances of :code:`quantity_value`.
Both units and quantities can be manipulated using standard arithmetic operators. 
For example, a :code:`quantity_value` representing density can be declared as follows:

.. code-block:: c++

    using density_type = maxwell::quantity_value<maxwell::si::kilogram_unit / maxwell::si::cubic_meter_unit>;

.. warning::

    If the underlying type is an integer type, truncation is possible when converting between quantity value instances.

Constructing Quantity Values 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There are several ways to construct a :code:`quantiy_value` instance. 

The simplest way is to provide a numeric value to the constructor.
 
.. code-block:: c++

    maxwell::si::meter<> length1{5.0}; // 5 meters

This constructs a :code:`quantity_value` representing 5 meters.

For more complex types, in-place construction can be used.  
Simply pass the arguments to the underlying numeric type to the constructor.

.. code-block:: c++ 

    maxwell::si::meter<std::complex<double>> length2{std::in_place, 3.0, 4.0}; // A vector of lengths

For more complex expressions, unit symbols can be used. 
Units are provided in the :code:`si`, :code:`us`, and :code:`other` namespaces. 

.. code-block:: c++ 

    using namespace maxwell:si::symbols;

    const quantity_value Kb = 5.670'374e-8 * W / (m2 * K * K * K * K); // Stefan-Boltzmann constant

:code:`quantity_value` instances support class template argument deduction (CTAD). 
This allows for more concise construction of quantity values; the units and quantity of the :code:`quantity_value` are deduced from the initialization expression;

.. code-block:: c++ 

    using namespace maxwell::si::symbols;

    const maxwell::quantity_value length3 = 10.0 * m; // length3 is quantity_value<meter_unit, length, double>

.. note::

    Maxwell does not provide user-defined literals for quantity values.

:code:`quantity_value` instances can also be constructed from other compatible quantity values.
If the underlying type of the quantity value being constructed is a floating point type, the construction is implicit. 
In this case, the unit conversion is calculated at compile-time, providing no run-time overhead compared to using the raw numeric type. 
The construction is checked for coherency at compile-time; an error is generated if the :code:`quantity_value` instances are not compatible.

.. code-block:: c++ 

    const maxwell::si::meter<> length4{1'000.0}; // 1000 meters 
    const maxwell::si::kilometer<> lenght5{length4}; // 1 kilometer, no run-time overhead

    const maxwell::si::ampere<> current{length4}; // Error - length and current are not compatible --- won't compile 

Maxwell directly computes the conversion factor between two units. 

.. code-block:: c++ 

    const maxwell::si::kilometer<> km{1.0}; 
    const maxwell::us::mile<> mile{km}; // 0.621371 miles, only one conversion factor is calculated. 
                               // Maxwell does not convert to base units first.                      

Maxwell also supports converting between linear and non-linear scales (e.g. decibels).

.. code-block:: c++

    const maxwell::si::decibel_milliwatt<> p1{30.0}; // 1 dbm 
    const maxwell::si::watt<> p2{p1}; // 1 watt, no run-time overhead

Quantities representing time can also be constructed from instances of :code:`std::chrono::duration`.

.. code-block:: c++

    const auto duration = std::chrono::seconds{120};
    const maxwell::si::second<> time{duration}; // 120 seconds

Type aliases for many common units are provided in the :code:`maxwell::si`, :code:`maxwell::us`, and :code:`maxwell::other` namespaces.
For a full list, see :doc:`predefined_units`.

.. important::

    When using type aliases, the :code:`<>` are required even if the default underlying type (:code:`double`) is used.
    Specifying a template parameter changes the underlying type. 

    .. code-block:: c++

        const si::meter<> length1{100.0}; // 100 meters (underlying type is double)
        const si::meter<int> length2{100}; // 100 meters (underlying type is int)

Operations on :code:`quantity_value`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 

A :code:`quantity_value` instance can be converted to another :code:`quantity_value` instance in different units using the :code:`in` function. 

.. code-block:: c++ 

    using namespace maxwell::si::symbols; 

    const maxwell::si::meter<> length1{1'000.0}; // 1000 meters
    const maxwell::si::kilometer<> length2 = length1.in(si::kilometer_unit); // length2 is 1 kilometer

Instances of :code:`quantity_value` representing dimensionless quantities can be implicitly converted to the underlying type. 

.. code-block:: c++ 

    const maxwell::si::dimensionless<> d{0.5}; 
    const double x = d; // x is 0.5 

    const maxwell::si::meter<> length3{10.0};
    const double y = length3; // Error - length is not dimensionless --- won't compile

Instances of :code:`quantity_value` support all arithetic and comparison operations supported by the underlying numeric type. 
These operations are checked at compile-time to ensure that they are only performed on compatible quantity values.
If necessary, unit conversions are performed automatically. 

.. code-block:: c++ 

    using namespace maxwell::si::symbols; 

    const maxwell::si::square_foot<> area = 5.0 * m * 5.0 * m + 100 * 100 * cm; // area is 64.5835 ft^2 

    const bool b = 100 * km > 60 * mi; // b is true

    const maxwell::quantity_value q = 5.0 * m + 10.0 * s // Error - length and time are not compatible --- won't compile

Note that arithetic operations can only be performed on quantity values whose units have the same reference point. 
For example, it is not permitted to mix different temperature scales such as Kelvin and Fahrenheit in arithmetic operations.

.. code-block:: c++ 
    
    const maxwell::si::kelvin<> t1{300.0};
    const maxwell::si::fahrenheit<> t2{80.33};

    const auto t3 = t1 + t2; // Error - Kelvin and Fahrenheit have different reference points --- won't compile

Ratios of :code:`quantity_values` of the same units are correctly identified as dimensionless quantities.

.. code-block:: c++ 

    const maxwell::si::meter<> length1{10.0};
    const maxwell::si::kilometer<> length2{1.0};

    const maxwell::si::dimensionless<> ratio = length1 / length2; // ratio is 0.01

It is not recommended to use :code:`auto` as the result of an arithmetic expression involving :code:`quantity_value` instances as the result may be unexpected. 

Most functions defined in header :code:`cmath` are also supported for :code:`quantity_value` instances.  
These functions are defined in the :code:`maxwell::math` namespace.
Trigonometric functions can take :code:`quantiy_value` instances in any angle units and will return the correct value. 

.. code-block:: c++ 

    const double sin1 = maxwell::math::sin(maxwell::si::degree<>{30.0}); // d is 0.5
    const double sin2 = maxwell::math::sin(maxwell::si::radian<>{std::numbers::pi / 6.0}); // d is also 6.0 

For inverse trignometric functions, two overloads are provided. 
Overloads ending in "d" return angles in degrees; other overloads return angles in radians.

.. code-block:: c++ 

    const maxwell::si::degree<> angle1 = maxwell::math::asind(0.5); // angle1 is 30 degrees
    const maxwell::si::radian<> angle2 = maxwell::math::asin(0.5); // angle2 is pi/6 radians

Maxwell also provides transcendental functions such as :code:`exp` and :code:`log`. 
These functions can only be applied to dimensionless quantity values.

.. code-block:: c++ 

    const maxwell::si::dimensionless<> x = std::exp(maxwell::si::meter<>{3.0} / maxwell::si::meter<>{2.0}); // x is e^(3/2)

.. warning:: 

    Integer division will be performed between :code:`quantity_value` instances if the underlying type of both instances is an integral type.

Instances of :code:`quantity_value` also support all comparison operators supported by the underlying numeric type.
These operations are checked at compile-time to ensure that they are only performed on :code:`quantity_value` instances of the same kind. 

.. code-block:: c++ 

    using namespace maxwell::si::symbols; 

    const maxwell::si::meter<> length1{100.0};
    const maxwell::si::kilometer<> length2{0.2};

    const bool b = length1 < length2; // b is true

Run-Time Mode 
------------- 

Introduction 
^^^^^^^^^^^^
Maxwell also supports specifying units at run-time using the :code:`quantity_holder` class template.
The :code:`quantity_holder` class template has the following declaration:

.. code-block:: c++

    template <auto Quantity, typename T = double>
    class quantity_holder;

In this declaration, :code:`Quantity` is an object that represents the kind of quantity the quantity holder belongs to. 
Like the :code:`quantity_value` class template, the :code:`Quantity` template parameter is a non-type template parameter. 

Constructing Quantity Holders 
-----------------------------

There are several ways to construct a :code:`quantity_holder` instance.
Most of the constructors are similar to those of :code:`quantity_value` except that the units must be specified in the constructor.
The units parameter is always the first parameter to the constructor. 
  
.. code-block:: c++ 

    // Can hold a length quantity with any units
    const maxwell::isq::length_holder<> l1{si::meter_unit, 100.0}; // 100 meters
    const maxwell::isq::length_holder<std::complex<double>> l2{si::meter_unit, std::in_place, 3.0, 4.0}; // 3.0 + 4.0i meters
    const maxwell::isq::length_holder<std::vector<double>> l3{si::foot_unit, std::in_place, {10.0, 20.0, 30.0}}; // vector of lengths in feet

When constructing from an instance of :code:`std::chrono::duration`, the units parameter is not required.

.. code-block:: c++ 

    const maxwell::isq::time_holder<> t1{std::chrono::seconds{120}}; // t1 holds 120 seconds

Instances of :code:`quantity_holder` can also be constructed from other compatible :code:`quantity_holder` instances.
The units stored in the :code:`quantity_holder` will be the units of the :code:`quantity_value` used to construct the :code:`quantity_holder`.
In this case, the units do not need to provided separately to the constructor. 

.. code-block:: c++ 

    const maxwell::isq::length_holder<> l4 = maxwell::si::meter<>{100.0}; // l4 holds 100 meters

Many type aliases for :code:`quantity_holder` instances that can hold common quantities are provided in the :code:`maxwell::isq` namespace.
For a complete list, see :doc:`predefined_units`.

Once the units of a :code:`quantity_holder` instance are specified, they cannot be changed. 
Assigning to a :code:`quantity_holder` instance from another :code:`quantity_holder` or :code:`quantity_value` instance does not change the units of the target instance.

.. code-block:: c++ 

    maxwell::isq::length_holder<> l5{si::meter_unit, 100.0}; // l5 holds 100 meters
    l5 = maxwell::si::kilometer<>{0.2}; // l5 now holds 200 meters (units unchanged)


:code:`quantity_holder` instances support class template argument deduction (CTAD). 
The quantity represented by the :code:`quantity_holder` is deduced from the initialization expression.

.. code-block:: c++ 

    const maxwell::quantity_holder l6 = maxwell::si::foot<>{10.0}; // l6 holds 10 feet

.. important:: 

    Unlike :code:`quantity_value`, instances of :code:`quantity_holder` are not trivially copyable if the underlying type is not trivially copyable.

Operations on :code:`quantity_holder`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
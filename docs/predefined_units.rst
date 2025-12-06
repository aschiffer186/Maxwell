Predefined Quantities and Units 
===============================

To make the library more ergonomic, Maxwell provides many predefined quantities in the namespace :code:`maxwell::isq` and 
predefined units and aliases for :code:`quantity_value` instances representing those units. 

Predefined Quantities 
--------------------
All predefined quantities are constants defined in the namespace :code:`Maxwell::isq`, e.g. :code:`maxwell::isq::length`.

All pre-defined quantities are incompatible; it is not possble to assign an instance of :code:`quantity_value` representing 
one predefined quantity to another. 

* :code:`length` - Length 
* :code:`mass` - Mass 
* :code:`time` - Time 
* :code:`current` - Current 
* :code:`temperature` - Temperature 
* :code:`amount` - Amount 
* :code:`luminosity` - Luminosity 
* :code:`dimensionless` - Dimensionless/Number
* :code:`plane_angle` - Plane Angle
* :code:`solid_angle` - Solid Angle
* :code:`frequency` - Frequency
* :code:`force` - Force
* :code:`pressure` - Pressure 
* :code:`work` - Work/Energy 
* :code:`power` - Power 
* :code:`charge` - Electrical Charge 
* :code:`potential` - Electrical Potential
* :code:`capacitance` - Electrical Capacitance 
* :code:`condudctance` - Electrical Conductance 
* :code:`magnetic_flux` - Magnetic Flux 
* :code:`area`- Area 
* :code:`volumne` - Volume 
* :code:`torque` - Torque 
* :code:`velocity` - Velocity 
* :code:`acceleration` - Acceleration

In the unlikely event the type of a quantity is needed, the corresponding type is given by :code:`<quantity_name>_quantity_type`, e.g. 
for :code:`length`, the corresponding quantity is :code:`length_quantity_type`.

Pre-Defined Units 
-----------------

Maxwell pre-defines many units and type aliases for :code:`quantity_value` instances using those units. 
All units have names ending in :code:`_unit`; the corresponding type aliases do not have the :code:`_unit` prefix. 
For example, for meter, the unit is named :code:`meter_unit`, the type alias for a :code:`quantity_value` representing a meter is :code:`meter`.

SI Units 
^^^^^^^^
All SI units and type aliases are defined in the :code:`maxwell::si` namespace. Although Maxwell, does not pre-define every metric prefix of every
unit, many commonly used prefixed units are pre-defined. Note that all type aliases are templates, so that the underlying representation type can be specified. 
The default underlying representation type is :code:`double` which can be specified by leaving the template parameter list empty.

.. code-block:: cpp

    si::meter<int> // Quantity value representing meters with underlying type int
    si::meter<> // Quantity value representing meters with underlying type double (default)
 
* :code:`meter, meter_unit`
* :code:`kilometer, kilometer_unit`
* :code:`centimeter, centimeter_unit`
* :code:`millimeter, millimeter_unit`
* :code:`gram, gram_unit`
* :code:`kilogram, kilogram_unit`
* :code:`second, second_unit`
* :code:`ampere, ampere_unit`
* :code:`kelvin, kelvin_unit`
* :code:`mole, mole_unit`
* :code:`candela, candela_unit` 
* :code:`radian, radian_unit`
* :code:`celsius, celsius_unit`
* :code:`degree, degree_unit`
* :code:`steradian, steradian_unit`
* :code:`hertz, hertz_unit`
* :code:`newton, newton_unit`
* :code:`pascal, pascal_unit`
* :code:`joule, joule_unit`
* :code:`newton_meter, newton_meter_unit`
* :code:`watt, watt_unit`
* :code:`coulomb, coulomb_unit`
* :code:`volt, volt_unit`
* :code:`ohm, ohm_unit`
* :code:`siemens, siemens_unit`
* :code:`farad, farad_unit`
* :code:`weber, weber_unit`
* :code:`tesla, tesla_unit`
* :code:`henry, henry_unit`
* :code:`lumen, lumen_unit`
* :code:`lux, lux_unit`
* :code:`becquerel, becquerel_unit`
* :code:`gray, gray_unit`
* :code:`sievert, sievert_unit`
* :code:`katal, katal_unit`
* :code:`square_meter, square_meter_unit`
* :code:`cubic_meter, cubic_meter_unit`
* :code:`liter, liter_unit`
* :code:`meter_per_second, meter_per_second_unit`
* :code:`meter_per_second_squared, meter_per_second_squared_unit`

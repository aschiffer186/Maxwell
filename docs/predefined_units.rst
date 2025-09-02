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
unit, many commonly used prefixed units are pre-defined. 

* :code:`meter, meter_unit`
* :code:`gram, gram_unit`
* :code:`kilogram, kilogram_unit`
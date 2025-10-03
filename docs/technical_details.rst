Technical Details 
=================

Introduction 
------------- 
This document provides an in-depth discussion of the implementation details of the Maxwell library. 
It is not intended as a users guide. 

Class Structure 
----------------  
The core classes that perform compile-time dimension analysis are :code:`dimension_product_type`, :code:`quantity_type`, and :code:`unit`. C
Class template :code:`dimension_product_type` prodivdes a compile-time representation of the dimensions of a quantity. 
Class template :code:`quantity_type` contains a :code:`dimension_product_type` and contains the kind of quantity (e.g., length, mass, time).
Class template :code:`unit` contains a :code:`quantity_type` and a multiplier and reference representing the relationship of the unit to the base units in the system. 


The core class templates make heavy use of two important utilitie classes defined in the :code:`maxwell::utilities` namespace: 
:code:`template_string` and :code:`rational_type`. 
Class template :code:`template_string` provides a way to store and manipulate strings at compile-time with no heap allocations. 
Class template :code:`rational_type` provides a way to represent rationßal numbers at compile-time.

Class Template :code:`dimension_product_type`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: c++ 

    template <utility::template_string Name, utility::rational auto Exponent> 
    struct dimension_type;

    template <dimension... Dimensions>
    struct dimension_product_type; 

Class template :code:`dimension_type` represents a single dimension, such as length or time.
It contains two template parameters, :code:`Name` and :code:`Exponent`. 
:code:`Name` is a :code:`template_string` representing the name of the dimension, e.g., "Length". 
:code:`Exponent` is a :code:`rational` representing the exponent of the dimension, e.g., 1 for length, -2 for per second squared, etc.


Class template :code:`dimension_product_type` represents a product of dimensions, such as length/time^2.
It contains a variadic list of :code:`dimension_type` instances as its template parameters. 
This ensures the dimensions that make up the product are known at compile-time, allowing for compile-time checking of dimensional correctness in calculations.
Each dimension product has a unique type; therefore, equality between instances of :code:`dimension_product_type` can be simply by comparing types. 

Multiplication and division of dimension products are performed using a merge algorithm.
It is assumged that the dimensions in each product are sorted by name. 
When multiplying two dimension products, the dimensions are merged together, adding exponents for dimensions with the same name. 
When dividing two dimension products, the dimensions are merged together, subtracting exponents for dimensions with the same name.

If a :code:`dimension_product_type` represents a number, it has no template parameters.
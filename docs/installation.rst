Building and Installation
========================= 

Installing Maxwell
------------------

The easiest way to install Maxwell is by cloning the repository from GitHub and installing using CMake. 

.. code-block:: bash

    git clone https://github.com/aschiffer186/Maxwell.git
    cd Maxwell 
    cmake --workflow --preset install

Once Maxwell has been installed, it can be discovered in a source package using :code:`find_package`. 
The Maxwell package provides the following helper variables: 
* :code:`Maxwell_INCLUDE_DIRS` - include directories needed for Maxwell. 

The package also provides a single target to link against: :code:`Maxwell::Maxwell`.

Building Tests 
--------------
The unit tests for Maxwell and be build and run using CMake from the top level Maxwell folder. 

.. code-block:: bash 

    cd Maxwell 
    cmake --workflow --preset run-tests

Building Documents 
------------------
The documents can be built using CMake from the top level Maxwell folder. 
The documentation requuires Doxygen and Sphinx to be built. 
Doxygen can be installed using your OS's package manager. 
Sphinx and other requirements can be installed using pip.

.. code-block:: bash 

    cd Maxwell 
    pip install -r docs/requirements.txt
    cmake --workflow --preset build-docs 

Doxygen documentation will be installed at :code:`<build-directory>/docs/Doxygen.`
All other documentation will be installed at :code:`<build-directory>/docs/documentation`
    
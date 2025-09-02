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
Maxwell's unit tests can be run from the main Maxwell directory using CMake. 
```
cd Maxwell 
cmake --workflow --preset run-tests
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


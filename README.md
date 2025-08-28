# Overview
Maxwell is a strongly typed units library for C++20 and later standards based on the ISQ 80000 units specification. It provides compile-time checking of and conversion between units. 

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
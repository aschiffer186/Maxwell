# Overivew 
Maxwell is a header only, strongly-typed units library for C++ 20. It performs unit checking and conversions at compile-time; if a program is well-formed then all of its units are consistent. It also performs automatic units conversions at compile-time, eliminating common errors such as converting between degrees and radians. 

A basic example is shown below
```
#include <Maxwell.hpp>

using namespace Maxwell;

int main()
{
    Foot ft{1.0}
    Inch in{ft};

    std::cout << in << "\n; //Prints "12 in"

    //PoundMass lbm{ft}; Compiler error!
    return 0;
}
```
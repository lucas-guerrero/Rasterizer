#include <iostream>
#include "vector.h"

using namespace aline;

int main() {
    
    Vector<int, 3> a;
    Vector<int, 3> b{ 0, 1, 2 };
    Vector<int, 3> c = a;
    c[0] = 3;
    c[1] = 4;
    c[2] = 5;
    Vector<int, 3> d = { 6 };

    try
    {
        int v = d.at(-1);
        std::cout << "Failure: d.at( -1 ) = " << v << std::endl;
    }
    catch (...) {}
    try
    {
        int v = d.at(3);
        std::cout << "Failure: d.at( 3 ) = " << v << std::endl;
    }
    catch (...) {}

    std::cout << "All complete" << std::endl;
}
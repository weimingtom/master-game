#include "events.hpp"

void f(int)
{
    std::cout << "f invoked.\n";
};


typedef srutil::delegate<void(int)> TestDelegate;

Slot<int> testFun()
{
    Slot<int> a1;
    return a1;
};


#include "events.hpp"

#include "Object.hpp"

void f(int)
{
    std::cout << "f invoked.\n";
};


typedef srutil::delegate<void(int)> TestDelegate;

Slot<int> testFun()
{
    Slot<int> a1;

    Slot2<obj_id_type,obj_id_type> handleMove;

    //handleMove.

    return a1;
};


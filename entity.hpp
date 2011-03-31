#ifndef __ENTITY_INCLUDED__
#define __ENTITY_INCLUDED__

#include <vector>
#include "math.h"
#include "globals.hpp"


class Entity
{
    public:
        vertex_tuple pos;
        Entity();
        Entity(int initX, int initY);
        void Draw();
        void Update(int time);
        void setPos(int X, int Y);
};

class Light: public Entity
{
    public:
        Light(int initX, int initY);
        void updateLightmap();
};


#endif


#ifndef __ENTITY_INCLUDED__
#define __ENTITY_INCLUDED__
#include "visilibity/visilibity.hpp"
class Entity
{
    public:
        VisiLibity::Point pos;
        Entity(float initX, float initY);
        void Draw();
        void Update(int time);
};

#endif


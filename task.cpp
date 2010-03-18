
#include "task.hpp"

goToPoint::goToPoint(float x,float y, Entity* ent)
{
    dest.set_x(x);
    dest.set_y(y);
    owner=ent;
    }

int goToPoint::execute(int time)
{
    float distInv=1/distance(dest,owner->pos);
    VisiLibity::Point dir = (dest-owner->pos)*distInv;
    owner->pos=owner->pos+dir*0.1;
    if (distance(dest,owner->pos)<0.001)
    return 1;
    return 0;
    };

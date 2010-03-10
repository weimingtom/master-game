#ifndef _TASK_INCLUDED_
#define _TASK_INCLUDED_

#include "entity.hpp"
#include "visilibity.hpp"
class Task
{
    public:
        Entity* owner;
        Task(){};
        virtual int execute(int time){};
};

class goToPoint: public Task {
        public:
        VisiLibity::Point dest;
        goToPoint(float x,float y, Entity* ent);
        int execute(int time);
    };

#endif

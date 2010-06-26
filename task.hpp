#ifndef _TASK_INCLUDED_
#define _TASK_INCLUDED_

#include "entity.hpp"
#include "visilibity/visilibity.hpp"
#include <vector>
class Task
{
    public:
        Entity* owner;
        Task(){};
        virtual int execute(int time)=0;
};

class ComplexTask: public Task {
    public:
        ComplexTask(Task &t1);
        std::vector<Task*> Actions;
        int currentAction;
        void AddAction(Task &t1);
        int execute(int time);
};


class goToPoint: public Task {
        public:
        VisiLibity::Point dest;
        goToPoint(float x,float y, Entity* ent);
        int execute(int time);
    };

#endif

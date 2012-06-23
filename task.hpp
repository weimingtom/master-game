#ifndef _TASK_INCLUDED_
#define _TASK_INCLUDED_


#include "entity.hpp"
#include "Rocket/Core/Types.h"
#include <vector>
#include <stdio.h>


class Entity;
class Task
{
    public:
        Object* owner;
        Task(){};
        virtual int execute(Rocket::Core::Time time) {}
        virtual int execute(obj_id_type target,obj_id_type sender) {}
};

class ComplexTask: public Task {
    public:
        ComplexTask();
        ComplexTask(Task &t1);
        std::vector<Task*> Actions;
        int currentAction;
        void AddAction(Task &t1);
        int execute(Rocket::Core::Time time);
};


class goToPoint: public Task {
        public:
        vertex_tuple dest;
        goToPoint();
        goToPoint(short x,short y, Object* ent);
        int execute(Rocket::Core::Time time);
    };


class visualSignal: public Task {
        public:
        vertex_tuple source;
        float radius;
        visualSignal();
        visualSignal(short x,short y, Object* ent);
        int execute(Rocket::Core::Time time);
    };

class move: public Task {
    public:
        move();
        move(obj_id_type target);
        int execute(obj_id_type target,obj_id_type sender);
    };

int handleMoveFun(obj_id_type target,obj_id_type sender);


#endif


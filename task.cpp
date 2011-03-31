#include "task.hpp"


ComplexTask::ComplexTask()
{
    currentAction=0;
}


ComplexTask::ComplexTask(Task &t1)
{
    AddAction(t1);
    currentAction=0;
}


int ComplexTask::execute(Rocket::Core::Time time)
{
//    std::vector<Task*>::iterator it=Actions.begin();

    if (!Actions[currentAction]->execute(time))
    {
        return 0;
    }
    else if (currentAction<Actions.size()-1)
    {
        currentAction++;
        return 0;
    }
    else
    return 1;

}

 void ComplexTask::AddAction(Task &t1)
 {
     Actions.push_back(&t1);
};

goToPoint::goToPoint()
{

};

goToPoint::goToPoint(short x,short y, Entity* ent)
{
    dest.first=x;
    dest.second=y;
    owner=ent;
    }

int goToPoint::execute(Rocket::Core::Time time)
{

    owner->pos.first=dest.first;
    owner->pos.second=dest.second;
    printf("%i,%i \n",owner->pos.first,owner->pos.second);

    return 1;
    };


visualSignal::visualSignal()
{

};

visualSignal::visualSignal(short x,short y, Entity* ent)
{
    radius=100;
    source.first=x;
    source.second=y;
    owner=ent;

};


int visualSignal::execute(Rocket::Core::Time time)
{
    return 0;

};



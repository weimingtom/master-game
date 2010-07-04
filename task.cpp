
#include "task.hpp"

ComplexTask::ComplexTask(Task &t1)
{
    AddAction(t1);
    currentAction=0;
}

ComplexTask::ComplexTask()
{
    currentAction=0;
}


int ComplexTask::execute(int time)
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
    if (distance(dest,owner->pos)<0.1)
        return 1;
    return 0;
    };

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
    float distInv=1/VisiLibity::distance(dest,owner->pos);
    VisiLibity::Point dir = (dest-owner->pos)*distInv;
    owner->pos=owner->pos+dir*0.1;
    if (VisiLibity::distance(dest,owner->pos)<0.1)
        return 1;
    return 0;
    };

visualSignal::visualSignal()
{

};

visualSignal::visualSignal(float x,float y, Entity* ent)
{
    radius=100;
    source.set_x(x);
    source.set_y(y);
    owner=ent;

};


int visualSignal::execute(int time)
{
    float dist=VisiLibity::distance(source,owner->pos);
    if (dist<radius)
    {
        static ComplexTask followPath;
        followPath = planPath(source.x(),source.y());
        UpdateTimerSlot.removeTask();
        UpdateTimerSlot.addTask<ComplexTask>(followPath);
        return 1;
    };
    return 0;

};


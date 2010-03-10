#ifndef _EVENTS_INCLUDED
#define _EVENTS_INCLUDED

#define SRUTIL_DELEGATE_PREFERRED_SYNTAX

#include "entity.hpp"
#include "task.hpp"

#include <iostream>
#include <vector>
#include <srutil/delegate/delegate.hpp>


using namespace std;



template <typename PARAM>
class Slot         //слот для наблюдателей с одним параметром
{
protected:
    typedef srutil::delegate<int (PARAM)> FastDelegate;
    typedef  FastDelegate Observer;
    typedef  std::vector<Observer> Observers;

    Observers m_Observers;

public:



//template <typename OBJECT, typename FUNCTION>
//void add(Entity & ent)
//  {
//      m_Observers.push_back(FastDelegate::template from_method<Entity,&Entity::Update>(&ent));
// }

template <typename TaskType>
void addTask(TaskType & tsk)
  {
      m_Observers.push_back(FastDelegate::template from_method<TaskType,&TaskType::execute>(&tsk));
 }


void operator() (PARAM p)
    {
        //Observers::iterator cii=m_Observers.begin();
        for (int i=0;i<m_Observers.size();i++)
        {
            m_Observers[i](p);
        };
    }

};


#endif

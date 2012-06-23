#ifndef _EVENTS_INCLUDED
#define _EVENTS_INCLUDED

#define SRUTIL_DELEGATE_PREFERRED_SYNTAX

#include "entity.hpp"
#include "task.hpp"

#include <iostream>
#include <vector>
#include <srutil/delegate/delegate.hpp>


using namespace std;

typedef std::string comp_id_type;

template <typename PARAM>
class Slot         //слот для наблюдателей с одним параметром
{
protected:
    typedef srutil::delegate<int (PARAM)> FastDelegate;
    typedef  FastDelegate Observer;
    typedef  std::vector<Observer> Observers;

public:
    Observers m_Observers;


template <typename TaskType>
void addTask(TaskType & tsk)
{
    m_Observers.push_back(FastDelegate::template from_method<TaskType,&TaskType::execute>(&tsk));
}


void removeTask()
{
    m_Observers.pop_back();
}

void clear()
{
    m_Observers.clear();
}


int operator() (PARAM p)
    {
       typename Observers::iterator cii=m_Observers.begin();
       for (;cii!=m_Observers.end();++cii)
        {
            Observer cio=*cii;
            int a=cio(p);
            if (a) return a;
        };
        return 0;
    }

};


template <typename PARAM1, typename PARAM2>
class Slot2         //слот для наблюдателей с 2 параметрами
{
protected:
    typedef srutil::delegate<int (PARAM1,PARAM2)> FastDelegate;
    typedef  FastDelegate Observer;
    typedef  std::vector<Observer> Observers;

public:
    Observers m_Observers;


template <typename TaskType>
void addTask(int (*f)(PARAM1,PARAM2))
{
    m_Observers.push_back(FastDelegate::template from_function(f));
}


void removeTask()
{
    m_Observers.pop_back();
}

void clear()
{
    m_Observers.clear();
}


int operator() (PARAM1 p1,PARAM2 p2)
    {
       typename Observers::iterator cii=m_Observers.begin();
       for (;cii!=m_Observers.end();++cii)
        {
            Observer cio=*cii;
            int a=cio(p1,p2);
            if (a) return a;
        };
        return 0;
    }

};


#endif

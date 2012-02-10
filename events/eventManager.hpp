#ifndef __EVENTMANAGER_HPP__
#define __EVENTMANAGER_HPP__
#include <map>
#include "Object.hpp"

#define SRUTIL_DELEGATE_PREFERRED_SYNTAX

#include <srutil/delegate/delegate.hpp>
#include <vector>

typedef std::string eventType;
typedef std::string eventArg;

struct gameEvent
{
    eventType type;
    std::map<std::string,eventArg> args;
};


typedef std::string event_notifier_id;
typedef std::string event_handler_id;

class eventManager {
public:
    typedef srutil::delegate<int (gameEvent)> eventHandler;
    typedef std::vector<event_handler_id> eventNotifier;
	typedef std::map<event_notifier_id,eventNotifier> eventNotifiers;
	typedef std::map<event_handler_id,eventHandler> eventHandlers;

	eventManager() {}
	~eventManager() {}

	std::map<obj_id_type,eventNotifiers> notifiersByObj;
	std::map<obj_id_type,eventHandlers> handlersByObj;

	static eventManager *getInstance() {
		if(mInstance == NULL)
			mInstance = new eventManager();
		return mInstance;
	}

	void registerHandler(eventHandler handler,event_handler_id handler_id, obj_id_type obj) {

        handlersByObj[obj].insert(std::pair<event_handler_id,eventHandler>(handler_id,handler));

	}

	void registerNotifier(eventNotifier notifier,event_notifier_id notifier_id, obj_id_type obj) {

        notifiersByObj[obj].insert(std::pair<event_notifier_id,eventNotifier>(notifier_id,notifier));

	}
	void Subscribe(event_notifier_id notifier_id,event_handler_id handler_id, obj_id_type obj)
	{
        notifiersByObj[obj][notifier_id].push_back(handler_id);
	}

	void Notify(event_notifier_id notifier_id, obj_id_type obj,gameEvent evt)
	{
	    eventNotifier::iterator i1;
        for (i1=notifiersByObj[obj][notifier_id].begin();i1!=notifiersByObj[obj][notifier_id].end();i1++)
        {
            handlersByObj[obj][*i1](evt);
        }
	}

protected:
	static eventManager *mInstance;
};

void testEventManager();
void registerEvents();

#endif

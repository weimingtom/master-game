
#ifndef ROCKETINVADERSEVENTHANDLERSTARTGAME_H
#define ROCKETINVADERSEVENTHANDLERSTARTGAME_H

#include "EventHandler.h"

/**
	@author Peter Curry
 */

class EventHandlerMode : public EventHandler
{
public:
	EventHandlerMode();
	virtual ~EventHandlerMode();

	virtual void ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value);
};

#endif

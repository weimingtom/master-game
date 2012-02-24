
#ifndef ROCKETINVADERSEVENTHANDLER_H
#define ROCKETINVADERSEVENTHANDLER_H

#include <Rocket/Core/String.h>

namespace Rocket {
namespace Core {

class Event;

}
}

/**
	@author Peter Curry
 */

class EventHandler
{
public:
	virtual ~EventHandler();
	virtual void ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value) = 0;
};


#endif

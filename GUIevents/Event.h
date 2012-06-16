
#ifndef ROCKETINVADERSEVENT_H
#define ROCKETINVADERSEVENT_H

#ifndef __MY_WIN32__
#undef __WIN32__
#undef _WIN32
#undef ROCKET_PLATFORM_WIN32
#define ROCKET_PLATFORM_UNIX
#endif

#include <Rocket/Core/EventListener.h>

/**
	@author Peter Curry
 */

class Event : public Rocket::Core::EventListener
{
public:
	Event(const Rocket::Core::String& value);
	virtual ~Event();

	/// Sends the event value through to Invader's event processing system.
	virtual void ProcessEvent(Rocket::Core::Event& event);

	/// Destroys the event.
	virtual void OnDetach(Rocket::Core::Element* element);

private:
	Rocket::Core::String value;
};

#endif

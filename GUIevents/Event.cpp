
#include "Event.h"
#include "EventManager.h"

Event::Event(const Rocket::Core::String& value) : value(value)
{
}

Event::~Event()
{
}

// Sends the event value through to Invader's event processing system.
void Event::ProcessEvent(Rocket::Core::Event& event)
{
	EventManager::ProcessEvent(event, value);
}

// Destroys the event.
void Event::OnDetach(Rocket::Core::Element* ROCKET_UNUSED(element))
{
	delete this;
}

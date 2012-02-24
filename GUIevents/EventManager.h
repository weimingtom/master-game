
#ifndef ROCKETINVADERSEVENTMANAGER_H
#define ROCKETINVADERSEVENTMANAGER_H

#include <Rocket/Core/Event.h>

class EventHandler;

/**
	@author Peter Curry
 */

class EventManager
{
public:
	/// Releases all event handlers registered with the manager.
	static void Shutdown();

	/// Registers a new event handler with the manager.
	/// @param[in] handler_name The name of the handler; this must be the same as the window it is handling events for.
	/// @param[in] handler The event handler.
	static void RegisterEventHandler(const Rocket::Core::String& handler_name, EventHandler* handler);

	/// Processes an event coming through from Rocket.
	/// @param[in] event The Rocket event that spawned the application event.
	/// @param[in] value The application-specific event value.
	static void ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value);
	/// Loads a window and binds the event handler for it.
	/// @param[in] window_name The name of the window to load.
	static bool LoadWindow(const Rocket::Core::String& window_name);

private:
	EventManager();
	~EventManager();
};


#endif

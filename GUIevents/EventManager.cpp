
#include "EventManager.h"
#include <Rocket/Core/Context.h>
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/ElementUtilities.h>
#include <Shell.h>
#include "EventHandler.h"
#include <map>

// The game's element context (declared in main.cpp).
extern Rocket::Core::Context* context;

// The event handler for the current screen. This may be NULL if the current screen has no specific functionality.
static EventHandler* event_handler = NULL;

// The event handlers registered with the manager.
typedef std::map< Rocket::Core::String, EventHandler* > EventHandlerMap;
EventHandlerMap event_handlers;

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

// Releases all event handlers registered with the manager.
void EventManager::Shutdown()
{
	for (EventHandlerMap::iterator i = event_handlers.begin(); i != event_handlers.end(); ++i)
		delete (*i).second;

	event_handlers.clear();
	event_handler = NULL;
}

// Registers a new event handler with the manager.
void EventManager::RegisterEventHandler(const Rocket::Core::String& handler_name, EventHandler* handler)
{
	// Release any handler bound under the same name.
	EventHandlerMap::iterator iterator = event_handlers.find(handler_name);
	if (iterator != event_handlers.end())
		delete (*iterator).second;

	event_handlers[handler_name] = handler;
}

// Processes an event coming through from Rocket.
void EventManager::ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value)
{

    if (event_handler != NULL)
    {
        event_handler->ProcessEvent(event, value);
    }
}

// Loads a window and binds the event handler for it.
bool EventManager::LoadWindow(const Rocket::Core::String& window_name)
{
	// Set the event handler for the new screen, if one has been registered.
	EventHandler* old_event_handler = event_handler;
	EventHandlerMap::iterator iterator = event_handlers.find(window_name);
	if (iterator != event_handlers.end())
		event_handler = (*iterator).second;
	else
		event_handler = NULL;

	// Attempt to load the referenced RML document.
	Rocket::Core::String document_path = Rocket::Core::String("./assets/") + window_name + Rocket::Core::String(".rml");
	Rocket::Core::ElementDocument* document = context->LoadDocument(document_path.CString());
	if (document == NULL)
	{
		event_handler = old_event_handler;
		return false;
	}

	// Set the element's title on the title; IDd 'title' in the RML.
	Rocket::Core::Element* title = document->GetElementById("title");
	if (title != NULL)
		title->SetInnerRML(document->GetTitle());

	document->Focus();
	document->Show();

	// Remove the caller's reference.
	document->RemoveReference();

	return true;
}

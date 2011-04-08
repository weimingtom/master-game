#include "EventHandlerMode.h"
#include <Rocket/Core/Context.h>
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/ElementUtilities.h>
#include <Rocket/Core/Event.h>
#include "EventManager.h"
#include "ElementMap.hpp"


// The game's element context (declared in main.cpp).
extern Rocket::Core::Context* context;

EventHandlerMode::EventHandlerMode()
{
}

EventHandlerMode::~EventHandlerMode()
{
}

void EventHandlerMode::ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value)
{

	if (value == "swap_mode")
	{

        Rocket::Core::Element* dest_element = event.GetTargetElement();

        Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();

        Rocket::Core::Element* answers = dest_element->GetParentNode();

        ElementMap* map =(ElementMap*)document->GetElementById("game");

        if (map->mode==1)
            map->mode=0;
        else
        {
            map->mode=1;
            //map->map->UpdateTimerSlot.removeTask();
        }


        printf("\n");
        printf("mode:");
        printf("%i",map->mode);
        printf("\n");

	}
}

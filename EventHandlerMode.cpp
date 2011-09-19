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


	if (value == "close")
    {
	    Rocket::Core::Element* dest_element = event.GetTargetElement();

        Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();

        document->Close();
	}


	if (value == "minimize")
    {
	    Rocket::Core::Element* dest_element = event.GetTargetElement();

        Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();



        Rocket::Core::Box box;
        box.SetContent(Rocket::Core::Vector2f(100, 10));
        box.SetEdge(Rocket::Core::Box::BORDER, Rocket::Core::Box::TOP, 1);
        //document->RemoveChild(document->GetElementById("window"));
        //document->SetBox(box);
        if (document->GetElementById("window")->IsVisible())
        {
            document->GetElementById("window")->SetProperty("visibility","hidden");
            document->SetProperty("height","10px");
        } else

        {
            document->GetElementById("window")->SetProperty("visibility","visible");
            document->SetProperty("height","200px");
        }
        document->Update();
	}


}

#include "EventHandlerMode.h"
#include <Rocket/Core/Context.h>
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/ElementUtilities.h>
#include <Rocket/Core/Event.h>
#include "EventManager.h"
#include "../ElementMap.hpp"


EventHandlerMode::EventHandlerMode()
{
}

EventHandlerMode::~EventHandlerMode()
{
}

void EventHandlerMode::ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value)
{
	if(value=="canalisation"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();

		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		Rocket::Core::Element *canalElement=document->GetElementById("canalisation_elements");
		if(canalElement!=NULL){
			canalElement->SetProperty("visibility", "visible");
			canalElement->SetProperty("width", "100%");
			canalElement->SetProperty("height", "100%");

		}
		Rocket::Core::Element *electrElement=document->GetElementById("electricity_elements");
		if(electrElement!=NULL){
			electrElement->SetProperty("visibility", "hidden");
			electrElement->SetProperty("width", "1px");
			electrElement->SetProperty("height", "1px");

		}
		Rocket::Core::Element *mapElement=document->GetElementById("map_elements");
		if(mapElement!=NULL){
			mapElement->SetProperty("visibility", "hidden");
			mapElement->SetProperty("width", "1px");
			mapElement->SetProperty("height", "1px");
		}
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=CANALISATION;
		map->mode_element=TUBE;
		/*document->RemoveChild(mapElement);
		document->InsertBefore(mapElement, canalElement);*/
	}
	else if(value=="map"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();

		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		Rocket::Core::Element *canalElement=document->GetElementById("canalisation_elements");
		if(canalElement!=NULL){
			canalElement->SetProperty("visibility", "hidden");
			canalElement->SetProperty("width", "1px");
			canalElement->SetProperty("height", "1px");
		}
		Rocket::Core::Element *electrElement=document->GetElementById("electricity_elements");
		if(electrElement!=NULL){
			electrElement->SetProperty("visibility", "hidden");
			electrElement->SetProperty("width", "1px");
			electrElement->SetProperty("height", "1px");

		}

		Rocket::Core::Element * mapElement=document->GetElementById("map_elements");
		if(mapElement!=NULL){
			mapElement->SetProperty("visibility", "visible");
			mapElement->SetProperty("width", "100%");
			mapElement->SetProperty("height", "100%");
		}
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=MAP;
		map->mode_element=WALL;
		/*document->RemoveChild(canalElement);
		document->InsertBefore(canalElement, mapElement);*/

	}
	else if(value=="electricity"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();

		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		Rocket::Core::Element *canalElement=document->GetElementById("canalisation_elements");
		if(canalElement!=NULL){
			canalElement->SetProperty("visibility", "hidden");
			canalElement->SetProperty("width", "1px");
			canalElement->SetProperty("height", "1px");
		}
		Rocket::Core::Element *electrElement=document->GetElementById("electricity_elements");
		if(electrElement!=NULL){
			electrElement->SetProperty("visibility", "visible");
			electrElement->SetProperty("width", "100%");
			electrElement->SetProperty("height", "100%");

		}

		Rocket::Core::Element * mapElement=document->GetElementById("map_elements");
		if(mapElement!=NULL){
			mapElement->SetProperty("visibility", "hidden");
			mapElement->SetProperty("width", "1px");
			mapElement->SetProperty("height", "1px");
		}
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=ELECTRICITY;
		map->mode_element=ALARM_MAGISTRAL;
		/*document->RemoveChild(canalElement);
		document->InsertBefore(canalElement, mapElement);*/

	}else if(value=="tube"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=CANALISATION;
		map->mode_element=TUBE;
	}else if(value=="valve"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=CANALISATION;
		map->mode_element=VALVE;

	}else if(value=="wall"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=MAP;
		map->mode_element=WALL;

	}else if(value=="res_gen_start"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=MAP;
		map->mode_element=RES_GEN_START;

	}else if(value=="gen"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=MAP;
		map->mode_element=GEN;

	}else if(value=="water_gen"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=MAP;
		map->mode_element=WATER_GEN;

	}else if(value=="control"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=MAP;
		map->mode_element=CONTROL;

	}else if(value=="pump"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=MAP;
		map->mode_element=PUMP;

	}else if(value=="light"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=MAP;
		map->mode_element=LIGHT;

	}else if(value=="outer_link"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=MAP;
		map->mode_element=OUTER_LINK;

	}else if(value=="patient"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=MAP;
		map->mode_element=PATIENT;

	}else if(value=="alarm_magistral"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=ELECTRICITY;
		map->mode_element=ALARM_MAGISTRAL;

	}else if(value=="reserve_magistral"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=ELECTRICITY;
		map->mode_element=RESERVE_MAGISTRAL;

	}else if(value=="div_corb"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=ELECTRICITY;
		map->mode_element=DIV_CORB;

	}else if(value=="corb"){
		Rocket::Core::Element* dest_element = event.GetTargetElement();
		Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
		ElementMap* map =(ElementMap*)document->GetElementById("game");
		map->mode=ELECTRICITY;
		map->mode_element=CORB;

	}

	/*if (value == "swap_mode")
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

	}*/


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

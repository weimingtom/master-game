
#undef __WIN32__
#undef _WIN32


#include "ElementMap.hpp"
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/Input.h>

#include <sstream>
#include "map.hpp"

#include <ShellOpenGL.h>

#include <GL/glu.h>


ElementMap::ElementMap(const Rocket::Core::String& tag) : Rocket::Core::Element(tag)
{
	map = new Map();
}

ElementMap::~ElementMap()
{
	delete map;
}

// Intercepts and handles key events.
void ElementMap::ProcessEvent(Rocket::Core::Event& event)
{
    Rocket::Core::Element::ProcessEvent(event);

    if (event=="mousemove")
    {
        Rocket::Core::Element* dest_element = event.GetTargetElement();

        Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();

        Rocket::Core::Element* coords = document->GetElementById("coords");

        int xcoord = event.GetParameter("mouse_x",0);
        int ycoord = event.GetParameter("mouse_y",0);



    GLdouble posX,posY,posZ;


    gluUnProject(xcoord,-ycoord + map->viewport[1] + map->viewport[3] +map->top,0,map->modelview,map->projection,map->viewport,&posX,&posY,&posZ);



        std::string s;
        std::stringstream out;
        out << posX << "," <<posY;
        s = out.str();
        Rocket::Core::String rs = Rocket::Core::String(s.c_str());

        Rocket::Core::Element* newText = document->CreateElement("p");
        newText->SetId("pc");

        coords->ReplaceChild((Rocket::Core::Element*)document->CreateTextNode(s.c_str()),coords->GetChild(0));

        //dest_element->Update();
    }

}


// Updates the game.
void ElementMap::OnUpdate()
{
    map->left=GetOwnerDocument()->GetAbsoluteLeft();

    map->top=GetOwnerDocument()->GetAbsoluteTop();

    map->width=GetOwnerDocument()->GetClientWidth();

    map->height=GetOwnerDocument()->GetClientHeight();

	map->Update();

}

// Renders the game.
void ElementMap::OnRender()
{

	map->Render();


}

void ElementMap::OnChildAdd(Rocket::Core::Element* element)
{
	Rocket::Core::Element::OnChildAdd(element);

	if (element == this){
        GetOwnerDocument()->AddEventListener("mousemove", this);
        GetOwnerDocument()->AddEventListener("load", this);
	}

}


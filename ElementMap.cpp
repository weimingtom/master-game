
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

    if (event=="mousedown")
    {


        vertex_tuple v1;
        v1.first=map->cursorX;
        v1.second=map->cursorY;

        if (mode)
        {

        int btn = event.GetParameter("button",0);
        if (btn==0)
            map->walls.insert(v1);
        else
            map->walls.erase(v1);
        } else

		if (map->walls.find(v1)==map->walls.end())
		{

            if (map->UpdateTimerSlot.m_Observers.size()==0)
            {
                static ComplexTask* followPath;
                followPath = map->planPath(map->cursorX,map->cursorY);
                if (followPath!=NULL)
                {
                    map->UpdateTimerSlot.addTask<ComplexTask>(*followPath);
                }
            }
        }


    }


    if (event=="mousemove")
    {
        Rocket::Core::Element* dest_element = event.GetTargetElement();

        Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();

        Rocket::Core::Element* coords = document->GetElementById("coords");

        int xcoord = event.GetParameter("mouse_x",0);
        int ycoord = event.GetParameter("mouse_y",0);



        GLdouble posX,posY,posZ;


        gluUnProject(xcoord,-ycoord + map->viewport[1] + map->viewport[3] +map->top,0,map->modelview,map->projection,map->viewport,&posX,&posY,&posZ);

        map->cursorX=posX;
        map->cursorY=posY;

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


    if (event=="swap_mode")
    {
        std::cout<< "swap mode \n";
    }

}


// Updates the game.
void ElementMap::OnUpdate()
{
    map->left=GetOwnerDocument()->GetElementById("content")->GetAbsoluteLeft();

    map->top=GetOwnerDocument()->GetElementById("content")->GetAbsoluteTop();

    map->width=GetOwnerDocument()->GetElementById("content")->GetClientWidth();

    map->height=GetOwnerDocument()->GetElementById("content")->GetClientHeight();

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
        GetOwnerDocument()->AddEventListener("mousedown", this);
        GetOwnerDocument()->AddEventListener("load", this);
	}

}


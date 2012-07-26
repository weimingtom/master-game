#ifndef __MY_WIN32__
#undef __WIN32__
#undef _WIN32
#endif


#include "ElementMap.hpp"
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/Input.h>

#include <sstream>
#include "map.hpp"
#include <Shell.h>
#include <ShellOpenGL.h>

#include <GL/glu.h>

#include "events/eventManager.hpp"



//#include "globals.hpp"

#ifndef round
#define	round(x)	((x-floor(x))>0.5 ? ceil(x) : floor(x))
#endif




ElementMap::ElementMap(const Rocket::Core::String& tag) : Rocket::Core::Element(tag)
{
	map = new Map();
	mode=MAP;
	mode_element=WALL;
}

ElementMap::~ElementMap()
{
	delete map;
}

// Intercepts and handles key events.
void ElementMap::ProcessEvent(Rocket::Core::Event& event)
{
    Rocket::Core::Element::ProcessEvent(event);

    // перетаскивание карты

    static float startPanX=0;
    static float startPanY=0;

    static float panCenterX=0;
    static float panCenterY=0;

    static bool pan = false;

    static GLint viewport[4];
    static GLdouble modelview[16];
    static GLdouble projection[16];



    int xcoord = event.GetParameter("mouse_x",0);
    int ycoord = event.GetParameter("mouse_y",0);


    if (event=="mousescroll")
    {
        int delta = event.GetParameter("wheel_delta",0);
        map->mapLeft=map->mapLeft-delta;
        map->mapTop=map->mapTop+delta;
        map->mapRight=map->mapRight+delta;
        map->mapBottom=map->mapBottom-delta;
    }


    if (GetOwnerDocument()->GetElementById("map")->IsPointWithinElement(Rocket::Core::Vector2f(xcoord,ycoord)))
    {

        if (event=="mouseup")
        {
            pan=false;

        };


        if (event=="mousedown")
        {
            if ((event.GetParameter("button",-1)==0)&&(event.GetParameter("ctrl_key",false)==true))
            {
                pan=true;
                GLdouble posX,posY,posZ;

                gluUnProject(xcoord,-(ycoord) + map->viewport[1] + map->viewport[3] +map->top,0,map->modelview,map->projection,map->viewport,&posX,&posY,&posZ);

                panCenterX=(map->mapRight+map->mapLeft)/2.0;
                panCenterY=(map->mapTop+map->mapBottom)/2.0;

                startPanX=posX;
                startPanY=posY;

                memcpy(viewport, map->viewport, sizeof(map->viewport));
                memcpy(modelview, map->modelview, sizeof(map->modelview));
                memcpy(projection, map->projection, sizeof(map->projection));

                //printf("%f,%f \n",panCenterX,panCenterX);
                //printf("%f,%f \n",startPanX,startPanY);

            }
            else if (event.GetParameter("button",-1)==1)
            {

                Rocket::Core::ElementDocument* debugWindow = context->LoadDocument("./assets/debugDialog.rml");
                if (debugWindow != NULL)
                {
                    debugWindow->Show();
                    debugWindow->RemoveReference();
                }
                debugWindow->SetId("debugWindow");


                Rocket::Core::Element* window = debugWindow->CreateElement("div");
                debugWindow->GetElementById("content")->AppendChild(window,true);
                //window->SetId("convTags");

                /*debugText->AppendChild(debugWindow->CreateTextNode("Текущее состояние диалога"),true);


                debugWindow->GetElementById("content")->AppendChild(debugText,true);
                */
                std::vector<Object*> objectsOnSpot = getObjectsWithCoords(map->cursorX,map->cursorY);
                rapidxml::xml_node<> *currentParameters = gameObjectsTable["guest"]->Serialize(map->doc);

                printf(currentParameters->name());

                Rocket::Core::Element* dest_element = event.GetTargetElement();
                Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
                //Rocket::Core::Element* window = document->GetElementById("fields");

                if(window!=NULL)
                {
                    while (window->HasChildNodes())
                    window->RemoveChild(window->GetFirstChild());

                    //rapidxml::xml_node<> *attr = currentParameters->first_node();
                    rapidxml::xml_node<> *curComp = currentParameters->first_node()->first_node();
                    while (curComp!=0)
                        {
                        printf("\n");
                        printf(curComp->name());


                        rapidxml::xml_attribute<> *attr = curComp->first_attribute();
                        while (attr!=0)
                        {
                            Rocket::Core::Element* label = document->CreateElement("p");
                            label->AppendChild((Rocket::Core::Element*) document->CreateTextNode(attr->name()),true);
                            window->AppendChild(label,true);

                            Rocket::Core::Element* newField = document->CreateElement("input");
                            newField->SetAttribute("type","text");
                            newField->SetAttribute("value",attr->value());
                            window->AppendChild(newField,true);
                            printf("\n");
                            printf(attr->name());
                            printf("\n");
                            printf(attr->value());
                            printf("\n");

                            attr=attr->next_attribute();
                        };
                        curComp=curComp->next_sibling();
                    };
                };

            }
                else
            {

            vertex_tuple v1;
            v1.first=map->cursorX;
            v1.second=map->cursorY;


            std::vector<Object*> objectsOnSpot = getObjectsWithCoords(map->cursorX,map->cursorY);
            if (!objectsOnSpot.empty())
            {
                gameEvent evt;
                evt.args.insert(std::pair<std::string,eventArg>("sender","Map"));
                evt.args.insert(std::pair<std::string,eventArg>("target",objectsOnSpot[0]->getID()));
                eventManager::getInstance()->Notify("toggleEvent",objectsOnSpot[0]->getID(),evt);
                /*
                rapidxml::xml_node<> *currentParameters = objectsOnSpot[0]->Serialize(map->doc);
                printf(currentParameters->name());

                Rocket::Core::Element* dest_element = event.GetTargetElement();
                Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();
                Rocket::Core::Element* window = document->GetElementById("fields");

				if(window!=NULL){
					while (window->HasChildNodes())
                        window->RemoveChild(window->GetFirstChild());


                //for (rapidxml::xml_attribute<> *attr = currentParameters->first_attribute();attr->next_attribute();attr=attr->next_attribute())
                rapidxml::xml_attribute<> *attr = currentParameters->first_attribute();
                 while (attr!=0)
                 {


                    Rocket::Core::Element* label = document->CreateElement("p");
                    label->AppendChild((Rocket::Core::Element*) document->CreateTextNode(attr->name()),true);
                    window->AppendChild(label,true);

                    Rocket::Core::Element* newField = document->CreateElement("input");
                    newField->SetAttribute("type","text");
                    newField->SetAttribute("value",attr->value());
                    window->AppendChild(newField,true);
                    printf("\n");
                    printf(attr->name());
                    printf("\n");
                    attr=attr->next_attribute();
                };
				}
                */

            }

			int btn = event.GetParameter("button",0);


			/*
			if(btn==0){
				map->try_insert_element(v1,mode, mode_element);
			}else{
				map->try_delete_element(v1, mode);
			}
			*/
            /*if (mode)
            {
                int btn = event.GetParameter("button",0);
                if (btn==0)
                {

                    map->walls.insert(v1);
                }
                else
                    map->walls.erase(v1);
            }*/


            /*else

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
            }*/
            //static Task* mt1 =new move();

            //map->handleMove.addTask(handleMoveFun);

            }

        }


        if (event=="mousemove")
        {


            if (pan)
            {

                GLdouble posX,posY,posZ;

                gluUnProject(xcoord,-(ycoord) + viewport[1] + viewport[3] +map->top,0,modelview,projection,viewport,&posX,&posY,&posZ);

                GLdouble mapWidth=map->mapRight-map->mapLeft;
                GLdouble mapHeight=map->mapTop-map->mapBottom;

                GLdouble newCenterX = panCenterX - (posX-startPanX);

                GLdouble newCenterY = panCenterY - (posY-startPanY);

                //printf("newCenterX: %f, newCenterY: %f \n",newCenterX,newCenterY);
                //printf("%f,%f \n",posX,posY);


                map->mapLeft=newCenterX - mapWidth/2.0;
                map->mapRight=newCenterX + mapWidth/2.0;

                map->mapTop=newCenterY + mapHeight/2.0;
                map->mapBottom=newCenterY - mapHeight/2.0;



            }
              else
            {
            Rocket::Core::Element* dest_element = event.GetTargetElement();

            Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();

            Rocket::Core::Element* coords = document->GetElementById("coords");


            GLdouble posX,posY,posZ;


            gluUnProject(xcoord,-ycoord + map->viewport[1] + map->viewport[3] +map->top,0,map->modelview,map->projection,map->viewport,&posX,&posY,&posZ);

            map->cursorX=round(posX);
            map->cursorY=floor(posY);

            }
        }
    };
}


// Updates the game.
void ElementMap::OnUpdate()
{
    map->left=GetOwnerDocument()->GetElementById("map")->GetAbsoluteLeft();

    map->top=GetOwnerDocument()->GetElementById("map")->GetAbsoluteTop();

    map->width=GetOwnerDocument()->GetElementById("map")->GetClientWidth();

    map->height=GetOwnerDocument()->GetElementById("map")->GetClientHeight();

	//map->Update();


    if (Shell::GetElapsedTime() - map->update_start >= map->update_freq)
    {
        /*
        if (mode==0)
        {
            map->Update();
        } else
        {
            map->UpdateTimerSlot.clear();
        }
        */
          map->Update();
    }


}

// Renders the game.
void ElementMap::OnRender()
{

	map->Render(mode, mode_element);


}

void ElementMap::OnChildAdd(Rocket::Core::Element* element)
{
	Rocket::Core::Element::OnChildAdd(element);

	if (element == this)
	{
        GetOwnerDocument()->AddEventListener("mousemove", this);
        GetOwnerDocument()->AddEventListener("mousedown", this);
        GetOwnerDocument()->AddEventListener("mouseup", this);
        GetOwnerDocument()->AddEventListener("mousescroll", this);
        GetOwnerDocument()->AddEventListener("load", this);
	}

}


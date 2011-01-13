
#include "Listeners.hpp"
#include <Rocket/Core/Element.h>

#include <Rocket/Core.h>

#include "dialog.hpp"

static ClickListener click_listener;

// Registers an element as being a container of draggable elements.

void ClickListener::RegisterClickableContainer(Rocket::Core::Element* element)
{
	element->AddEventListener("click", &click_listener);
}

void ClickListener::ProcessEvent(Rocket::Core::Event& event)
{
    Rocket::Core::Element* dest_element = event.GetTargetElement();

    Rocket::Core::ElementDocument* document  = dest_element->GetOwnerDocument();

    Rocket::Core::Element* answers = dest_element->GetParentNode();

    while (answers->HasChildNodes())
    {
        answers->RemoveChild(answers->GetChild(0));
    }

    Rocket::Core::Element* convText =document->GetElementById("content")->GetElementById("convText");

    //convText->RemoveChild(convText->GetChild(0));
    Rocket::Core::Element* newText = document->CreateElement("p");
    newText->SetId("pc");

    newText->AppendChild(document->CreateTextNode(chooseAnswer(dest_element->GetAttribute("orderNum")->Get<int>())),true);

    convText->AppendChild(newText,true);

    newText = document->CreateElement("p");
    newText->SetId("npc");

    newText->AppendChild(document->CreateTextNode(chooseAnswer(0)),true);

    convText->AppendChild(newText,true);

    std::vector<char*> ansList=currentAnswers();


    int i=0;
    for (std::vector<char*>::iterator curText=ansList.begin(); curText<ansList.end();curText++)
    {


        Rocket::Core::Element* content = document->CreateElement("p");
        content->SetId("pc");

        content->AppendChild(document->CreateTextNode(*curText),true);
        answers->AppendChild(content,true);

        content->SetAttribute("orderNum",i);
        i++;


        MouseOverListener::RegisterMouseOverContainer(content);
        MouseOutListener::RegisterMouseOutContainer(content);
        ClickListener::RegisterClickableContainer(content);

    }

}

static MouseOverListener mouseover_listener;

void MouseOverListener::RegisterMouseOverContainer(Rocket::Core::Element* element)
{
	element->AddEventListener("mouseover", &mouseover_listener);
}

void MouseOverListener::ProcessEvent(Rocket::Core::Event& event)
{
		Rocket::Core::Element* dest_element = event.GetTargetElement();

		dest_element->SetId("select");
		dest_element->Update();
}


static MouseOutListener mouseout_listener;

void MouseOutListener::RegisterMouseOutContainer(Rocket::Core::Element* element)
{
	element->AddEventListener("mouseout", &mouseout_listener);
}

void MouseOutListener::ProcessEvent(Rocket::Core::Event& event)
{
		Rocket::Core::Element* dest_element = event.GetTargetElement();

		dest_element->SetId("pc");
		dest_element->Update();
}

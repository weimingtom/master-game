
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

    addTags(getConvNode(getCurNode()).effPlus);
    removeTags(getConvNode(getCurNode()).effMin);

    newText->AppendChild(document->CreateTextNode(chooseAnswer(0)),true);

    convText->AppendChild(newText,true);

    addTags(getConvNode(getCurNode()).effPlus);
    removeTags(getConvNode(getCurNode()).effMin);

    std::vector<char*> ansList=currentAnswers();


    int i=0;
    for (std::vector<char*>::iterator curText=ansList.begin(); curText<ansList.end();curText++)
    {
        dialogNode curNode=getConvNode(getConvNode(getCurNode()).children[i]);
        tags curState=getTags();
        if (includes(curState.begin(),curState.end(),curNode.precond.begin(),curNode.precond.end()) &&(curNode.active))
        {

            Rocket::Core::Element* content = document->CreateElement("p");
            content->SetId("pc");

            content->AppendChild(document->CreateTextNode(*curText),true);
            answers->AppendChild(content,true);

            content->SetAttribute("orderNum",i);


            MouseOverListener::RegisterMouseOverContainer(content);
            MouseOutListener::RegisterMouseOutContainer(content);
            ClickListener::RegisterClickableContainer(content);
        }
        i++;

    }



    Rocket::Core::ElementDocument* debugWindow = dest_element->GetContext()->GetDocument("debugWindow");

    Rocket::Core::Element* convTags = debugWindow->GetElementById("convTags");

    cout<<getTagsAsString();

    convTags->ReplaceChild(debugWindow->CreateTextNode(getTagsAsString().c_str()),convTags->GetFirstChild());
   // convTags->ReplaceChild(debugWindow->CreateTextNode(getTags().pop_front()),convTags->GetFirstChild());




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

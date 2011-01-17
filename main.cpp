#undef __WIN32__
#undef _WIN32
//#define ROCKET_PLATFORM_UNIX

#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include <Rocket/Core/Types.h>
#include <Input.h>
#include <Shell.h>
#include "SystemInterface.h"

#include "dialog.hpp"

#include "Listeners.hpp"





Rocket::Core::Context* context = NULL;

void GameLoop()
{
	glClear(GL_COLOR_BUFFER_BIT);

	context->Update();
	context->Render();

	Shell::FlipBuffers();
}

//#if defined ROCKET_PLATFORM_WIN32
//#include <windows.h>
//int APIENTRY WinMain(HINSTANCE ROCKET_UNUSED(instance_handle), HINSTANCE ROCKET_UNUSED(previous_instance_handle), char* ROCKET_UNUSED(command_line), int ROCKET_UNUSED(command_show))

int main(int ROCKET_UNUSED(argc), char** ROCKET_UNUSED(argv))
{
	// Generic OS initialisation, creates a window and attaches OpenGL.
	if (!Shell::Initialise("Master") ||
		!Shell::OpenWindow("Master", true))
	{
		Shell::Shutdown();
		return -1;
	}

	// Rocket initialisation.
	ShellRenderInterfaceOpenGL opengl_renderer;
	Rocket::Core::SetRenderInterface(&opengl_renderer);

	// Initialise our system interface to write the log messages to file.
	SystemInterface system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	Rocket::Core::Initialise();



	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(1024, 768));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		Shell::Shutdown();
		return -1;
	}

	Rocket::Debugger::Initialise(context);
	Input::SetContext(context);

	//Shell::LoadFonts(".\\assets\\");
	//Rocket::Core::FontDatabase::LoadFontFace(".\\assets\\arial.ttf");
	Rocket::Core::FontDatabase::LoadFontFace(".\\assets\\CONSOLA.TTF");
	// Load and show the demo document.
	Rocket::Core::ElementDocument* document = context->LoadDocument(".\\assets\\dialog.rml");
	if (document != NULL)
	{
		document->Show();
		document->RemoveReference();


	}

		Rocket::Core::ElementDocument* debugWindow = context->LoadDocument(".\\assets\\debugDialog.rml");
	if (debugWindow != NULL)
	{
		debugWindow->Show();
		debugWindow->RemoveReference();
		document->SetOffset(Rocket::Core::Vector2f(100,100),document);
	}
	debugWindow->SetId("debugWindow");



    Rocket::Core::Element* debugText = debugWindow->CreateElement("div");
	debugText->SetId("convTags");

//	Rocket::Core::Element* newTag = debugWindow->CreateElement("p");

//	debugText->AppendChild(newTag,true)

	debugText->AppendChild(debugWindow->CreateTextNode("Текущее состояние диалога"),true);


	debugWindow->GetElementById("content")->AppendChild(debugText,true);



    //chooseAnswer(0);
	Rocket::Core::Element* text = document->CreateElement("div");
	text->SetId("convText");


	Rocket::Core::Element* newText = document->CreateElement("p");
	newText->SetId("npc");

	newText->AppendChild(document->CreateTextNode(chooseAnswer(0)),true);

	text->AppendChild(newText,true);

	document->GetElementById("content")->AppendChild(text,true);

    std::vector<char*> ansList=currentAnswers();

    Rocket::Core::Element* answers = document->CreateElement("div");

    document->GetElementById("content")->AppendChild(answers,true);


    int i=0;
    for (std::vector<char*>::iterator curText=ansList.begin(); curText!=ansList.end();curText++)
    {
        dialogNode curNode=getConvNode(getConvNode(getCurNode()).children[i]);
        tags curState=getTags();
        if ((curState.empty() && curNode.precond.empty()) || (includes(curState.begin(),curState.end(),curNode.precond.begin(),curNode.precond.end())))
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

	Shell::EventLoop(GameLoop);

	// Shutdown Rocket.
	context->RemoveReference();
	Rocket::Core::Shutdown();

	Shell::CloseWindow();
	Shell::Shutdown();

	return 0;
}

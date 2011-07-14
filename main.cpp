#undef __WIN32__
#undef _WIN32
#undef ROCKET_PLATFORM_WIN32
#define ROCKET_PLATFORM_UNIX

#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include <Rocket/Core/Types.h>
#include <Rocket/Controls.h>

#include <Input.h>
#include <Shell.h>
#include "SystemInterface.h"

#include "dialog.hpp"

#include "Listeners.hpp"

#include "ElementMap.hpp"
#include "map.hpp"

#include "EventInstancer.h"
#include "EventManager.h"
#include "EventHandlerMode.h"

#include "Rocket/Core/Decorator.h"

#include "globals.hpp"



	GLfloat vertices[] = {500.0f,500.0f,0.1f,500.0f,0.0f,0.1f,0.0f,500.0f,0.1f};
    GLubyte indices[] = {0,1,2};



typedef std::pair<short,short> vertex_tuple;



Rocket::Core::Context* context = NULL;



void GameLoop()
{



    /*
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, indices);
	//glDrawArrays(GL_TRIANGLES, 0, 1);

	glDisableClientState(GL_VERTEX_ARRAY);
*/
	context->Update();
    glClear(GL_COLOR_BUFFER_BIT);
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

    Rocket::Controls::Initialise();




	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(screen_width, screen_height));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		Shell::Shutdown();
		return -1;
	}

	Rocket::Debugger::Initialise(context);
	Input::SetContext(context);


    string dir = string(".\\assets\\");
    vector<string> files = vector<string>();

    getDir(dir,files);

    for (unsigned int i = 0;i < files.size();i++) {

    std::vector<std::string> filename = split(files[i],'.');
        if (filename[1].compare("ttf")==0)
        {
            cout << files[i] << endl;
            files[i].insert(0,dir);
            Rocket::Core::FontDatabase::LoadFontFace(files[i].c_str());
        };
    };


	Rocket::Core::FontDatabase::LoadFontFace(".\\assets\\arial.ttf");
	Rocket::Core::FontDatabase::LoadFontFace(".\\assets\\CONSOLA.TTF");

	//Shell::LoadFonts(".\\assets\\");


	Rocket::Core::ElementInstancer* element_instancer = new Rocket::Core::ElementInstancerGeneric< ElementMap >();
	Rocket::Core::Factory::RegisterElementInstancer("game", element_instancer);
	element_instancer->RemoveReference();

    EventInstancer* event_instancer = new EventInstancer();
	Rocket::Core::Factory::RegisterEventListenerInstancer(event_instancer);
	event_instancer->RemoveReference();

	EventManager::RegisterEventHandler("game", new EventHandlerMode());

	// Load and show the demo document.
	Rocket::Core::ElementDocument* document = context->LoadDocument(".\\assets\\dialog.rml");
	if (document != NULL)
	{
		document->Show();
		document->RemoveReference();


	}



    //Rocket::Core::ElementDocument* game = context->LoadDocument(".\\assets\\game.rml");
	/*
	if (game != NULL)
	{
		game->Show();
		game->RemoveReference();
		document->SetOffset(Rocket::Core::Vector2f(400,100),document);
	}
	*/

	//Rocket::Core::Element* b1 = game->CreateElement("button");
	//game->AppendChild(b1,true);
    //game->AddEventListener("mousemove", this);
    //MapListener::RegisterMap(game);


		Rocket::Core::ElementDocument* debugWindow = context->LoadDocument(".\\assets\\debugDialog.rml");
	if (debugWindow != NULL)
	{
		debugWindow->Show();
		debugWindow->RemoveReference();
	}
	debugWindow->SetId("debugWindow");

    //debugWindow->IterateDecorators

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

    addTags(getConvNode(getCurNode()).effPlus);
    removeTags(getConvNode(getCurNode()).effMin);


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
    EventManager::LoadWindow("game");
	Shell::EventLoop(GameLoop);

	// Shutdown Rocket.
	context->RemoveReference();

	EventManager::Shutdown();
	Rocket::Core::Shutdown();

	Shell::CloseWindow();
	Shell::Shutdown();

	return 0;
}

#undef __MY_WIN32__
#ifndef __MY_WIN32__
#undef __WIN32__
#undef _WIN32
#undef ROCKET_PLATFORM_WIN32
#define ROCKET_PLATFORM_UNIX
#endif

#if defined ROCKET_PLATFORM_WIN32
#include <windows.h>
#endif

#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include <Rocket/Core/Types.h>
#include <Rocket/Controls.h>

//#include <GL/glew.h>


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

#include "Rocket/Core/StreamMemory.h"
#include "Rocket/Core/Factory.h"








typedef std::pair<short,short> vertex_tuple;



Rocket::Core::Context* context = NULL;

Rocket::Core::TextureHandle wallpaperTexture;
Rocket::Core::Vector2i wallpaperTexture_dimensions;




void GameLoop()
{


	context->Update();

    glClear(GL_COLOR_BUFFER_BIT);


    //glDisable(GL_TEXTURE_2D);  //без этого не отображается


	glViewport(0,0,screen_width,screen_height);

	float vertices[8]={0,screen_height,0,0,screen_width,0,screen_width,screen_height};
    //float texcoords[8]={0.2,0.6,0.2,0.2,0.6,0.2,0.6,0.6};
    float texcoords[8]={0.0,1.0,0.0,0.0,1.0,0.0,1.0,1.0};
    unsigned char colors[32] = {255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255};


    glPushMatrix();

    glVertexPointer(2, GL_FLOAT,0, vertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, (GLuint) wallpaperTexture);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT,0, texcoords);

    unsigned int indices[6]={0,1,2,2,3,0};

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

    //glDisable(GL_TEXTURE_RECTANGLE_ARB);
    glDisable(GL_TEXTURE_2D);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();





	context->Render();



	Shell::FlipBuffers();
}

/*#if defined ROCKET_PLATFORM_WIN32
int APIENTRY WinMain(HINSTANCE ROCKET_UNUSED(instance_handle), HINSTANCE ROCKET_UNUSED(previous_instance_handle), char* ROCKET_UNUSED(command_line), int ROCKET_UNUSED(command_show))
#else*/
int main(int ROCKET_UNUSED(argc), char** ROCKET_UNUSED(argv))
//#endif
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


    string dir = string(".\\assets\\*");
    vector<string> files = vector<string>();

    getDir(dir,files);

    for (unsigned int i = 0;i < files.size();i++) {

    std::vector<std::string> filename = split(files[i],'.');
	if(filename.size()!=2)
		continue;
        if (filename[1].compare("ttf")==0)
        {
            cout << files[i] << endl;
            files[i].insert(0,dir,0,dir.length()-1); //без звездочки
            Rocket::Core::FontDatabase::LoadFontFace(files[i].c_str());
        };
    };
	Rocket::Core::FontDatabase::LoadFontFace(".\\assets\\arial.ttf");
	//Rocket::Core::FontDatabase::LoadFontFace(".\\assets\\CONSOLA.TTF");


	Rocket::Core::GetRenderInterface()->LoadTexture(wallpaperTexture, wallpaperTexture_dimensions, ".\\assets\\wallpaper.tga");
    printf("texture width:%i \n",wallpaperTexture_dimensions.x);
    printf("texture height:%i \n",wallpaperTexture_dimensions.y);

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


		Rocket::Core::ElementDocument* debugWindow = context->LoadDocument(".\\assets\\debugDialog.rml");
	if (debugWindow != NULL)
	{
		debugWindow->Show();
		debugWindow->RemoveReference();
	}
	debugWindow->SetId("debugWindow");


    Rocket::Core::Element* debugText = debugWindow->CreateElement("div");
	debugText->SetId("convTags");

	debugText->AppendChild(debugWindow->CreateTextNode("Текущее состояние диалога"),true);


	debugWindow->GetElementById("content")->AppendChild(debugText,true);

    //chooseAnswer(0);
	Rocket::Core::Element* text = document->CreateElement("div");
	text->SetId("convText");

    Rocket::Core::Element* answers = document->CreateElement("div");
    answers->SetId("answers");



	Rocket::Core::Element* newText = document->CreateElement("p");
	newText->SetId("pc");

	document->GetElementById("content")->AppendChild(text,true);

    document->GetElementById("content")->AppendChild(answers,true);

    answers->AppendChild(newText,true);

    ClickListener::RegisterClickableContainer(newText);

    Rocket::Core::Dictionary parameters;
    parameters.Set("mouse_x", 0);
    parameters.Set("mouse_y", 0);
    parameters.Set("button", 0);

    newText->SetAttribute("orderNum",0);

    newText->DispatchEvent("click", parameters);


/*
	newText->AppendChild(document->CreateTextNode(chooseAnswer(0)),true);



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
            //Rocket::Core::Element* content;


            //content->AppendChild(document->CreateTextNode(*curText),true);
            Rocket::Core::String RMLtext (*curText);
            //content->ResolveProperty
            //Rocket::Core::StreamMemory s1 =  Rocket::Core::StreamMemory((Rocket::Core::byte*)*curText, sizeof(char)*strlen(*curText));
            //printf("%i \n",s1.Length());
            content->SetInnerRML(RMLtext);
            //bool parsed = Rocket::Core::Factory::InstanceElementStream(content,&s1);

            printf("%i \n",answers->GetNumChildren());

            answers->AppendChild(content,true);

            content->SetAttribute("orderNum",i);

            MouseOverListener::RegisterMouseOverContainer(content);
            MouseOutListener::RegisterMouseOutContainer(content);
            ClickListener::RegisterClickableContainer(content);
        }
        i++;

    }

    */
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


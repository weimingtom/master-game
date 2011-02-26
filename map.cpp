

#undef ROCKET_PLATFORM_WIN32
#undef __WIN32__
#undef _WIN32

#include "map.hpp"
#include <Rocket/Core.h>
#include <Shell.h>

#include <iostream>

// The game's element context (declared in main.cpp).
extern Rocket::Core::Context* context;



Map::Map()
{

    Rocket::Core::Vector2i texture_dimensions;
	Rocket::Core::GetRenderInterface()->LoadTexture(texture, texture_dimensions, "assets/invaders.tga");

}

Map::~Map()
{

}

void Map::Initialise()
{

}

void Map::Update()
{

}

void Map::Render()
{
   glViewport(left,768 - height - top,width,height);

   glMatrixMode (GL_MODELVIEW);

   glPushMatrix();

    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(-10,10,-10,10);


    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);

    glGetIntegerv(GL_VIEWPORT,viewport) ;



   glEnable(GL_TEXTURE_2D);

   	glBindTexture(GL_TEXTURE_2D, (GLuint) texture);

	//glColor4ub(255, 255, 255, 255);
	glBegin(GL_LINES);
        glVertex2f(-200.0f,0.0f);
        glVertex2f(200.0f,0.0f);
        glVertex2f(0.0f,200.0f);
        glVertex2f(0.0f,-200.0f);
	glEnd();


	glBegin(GL_QUADS);
	std::set<vertex_tuple>::iterator i1;
	for (i1=walls.begin();i1!=walls.end();i1++)
    {
        glVertex2f(i1->first-0.5,i1->second-0.5);
        glVertex2f(i1->first-0.5,i1->second+0.5);
        glVertex2f(i1->first+0.5,i1->second+0.5);
        glVertex2f(i1->first+0.5,i1->second-0.5);
    }
	glEnd();


	glPopMatrix();

    glMatrixMode (GL_MODELVIEW);

	glPopMatrix();

	glViewport(0,0,1024,768);

}






#undef ROCKET_PLATFORM_WIN32
#undef __WIN32__
#undef _WIN32

#include "map.hpp"
#include <Rocket/Core.h>
#include <Shell.h>




// The game's element context (declared in main.cpp).
extern Rocket::Core::Context* context;



Map::Map()
{

    Rocket::Core::Vector2i texture_dimensions;
	Rocket::Core::GetRenderInterface()->LoadTexture(texture, texture_dimensions, "assets/invaders.tga");
    Initialise();
}

Map::~Map()
{
Finalise();
}

void Map::Initialise()
{

    std::ifstream in(".\\assets\\map.xml");
    if (!in){
        printf("error opening file");
    }
    std::string s;
    std::string s1="";

    while(getline(in, s)) { // Discards newline char
    s1=s1+s+"\n";
    }
    in.close();


    xml_copy.assign(s1.begin(), s1.end());
    xml_copy.push_back('\0');

    //rapidxml::xml_document<> doc;    // character type defaults to char
    doc.parse<0>(&xml_copy[0]);    // 0 means default parse flags

    //std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

    rapidxml::xml_node<char> *v;
    v=doc.first_node("walls");
    std::string coords = v->first_attribute("coords")->value();
    std::vector<std::string> pairs = split(coords,' ');

    for (std::vector<std::string>::iterator p1 = pairs.begin(); p1!=pairs.end();p1++)
    {
        std::vector<std::string> xy = split(*p1,',');

        vertex_tuple v1;

        v1.first=atoi(xy[0].c_str());
        v1.second=atoi(xy[1].c_str());

        walls.insert(v1);
    }

}

void Map::Finalise()
{


	std::set<vertex_tuple>::iterator i1;
	std::stringstream ws;
	std::string wt;
	for (i1=walls.begin();i1!=walls.end();i1++)
    {
        ws << i1->first << "," << i1->second << " ";
    }

    std::getline(ws,wt);
    //ws >> wt;
    std::ofstream mapfile;

    mapfile.open (".\\assets\\map.xml");

    rapidxml::xml_attribute<> *attr = doc.allocate_attribute("coords", wt.c_str());

    rapidxml::xml_node<char> *v;

    v=doc.first_node("walls");

    rapidxml::xml_attribute<> *old_coords = v->first_attribute("coords");

    v->remove_attribute(old_coords);

    v->append_attribute(attr);

    mapfile << doc;


    mapfile.close();

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




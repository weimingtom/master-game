
#ifndef __MY_WIN32__
#undef ROCKET_PLATFORM_WIN32
#undef __WIN32__
#undef _WIN32
#endif

#include "map.hpp"
#include <Rocket/Core.h>
#include <cmath>
#include <Shell.h>

#include "globals.hpp"

#include "Object.hpp"
#include "ObjectTemplate.hpp"
#include "ObjTemplateMgr.hpp"

#include "ComponentTemplate.hpp"
#include "components/CompPhysTemplate.hpp"
#include "components/CompVisualSqTemplate.hpp"
#include "components/CompTogglableTemplate.hpp"
#include "CompTemplateMgr.hpp"

#include "sprites.hpp"

#include "events/eventManager.hpp"

#ifdef __MY_NOT__
#define not !
#endif

// The game's element context (declared in main.cpp).
extern Rocket::Core::Context* context;


Map::Map()
{


	Rocket::Core::GetRenderInterface()->LoadTexture(texture, texture_dimensions, "assets/invader.tga");

	/*
	if_res_gen_start=false;
	if_gen=false;
	if_water_gen=false;
	if_control=false;
	if_pump=false;
	if_light=false;
	if_outer_link=false;
	if_patient=false;
	*/
    Initialise();
}

Map::~Map()
{
Finalise();
}

void Map::Initialise()
{

    update_start=0;
	// How often the invaders move
	update_freq = 0.2;


    mapLeft=-15;
    mapRight=15;
    mapTop=15;
    mapBottom=-15;

    //загружаем карту

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

    rapidxml::xml_node<char> *v;


    //загружаем стены

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


    //загружаем шаблоны компонентов
    //сначала корневые


    rapidxml::xml_node<char> *templ = doc.first_node("goc");

    while (templ)
    {
        if (not (strcmp(templ->first_attribute("name")->value(),"CompPhys")))
            {
                comp_id_type compName = comp_id_type("CompPhys");
                CompTemplateMgr::getInstance()->registerRootTemplateNode(compName,templ);
            }

        if (not (strcmp(templ->first_attribute("name")->value(),"CompVisualSq")))
            {
                comp_id_type compName = comp_id_type("CompVisualSq");
                CompTemplateMgr::getInstance()->registerRootTemplateNode(compName,templ);

            }

        if (not (strcmp(templ->first_attribute("name")->value(),"CompTogglable")))
            {

                comp_id_type compName = comp_id_type("CompTogglable");
                CompTemplateMgr::getInstance()->registerRootTemplateNode(compName,templ);

            }


        templ=templ->next_sibling("goc");
    }


    //теперь из описаний объектов

    rapidxml::xml_node<char> *ob = doc.first_node("object_template");


    while (ob)
    {

    rapidxml::xml_node<char> *obComps = ob->first_node("components");
    rapidxml::xml_node<char> *templ = obComps->first_node("goc");

        while (templ)

        {

            if (not (strcmp(templ->first_attribute("name")->value(),"CompPhys")))
                {
                    comp_id_type compName = comp_id_type("CompPhys");
                    ComponentTemplate *compPhysTemplate2 = new CompPhysTemplate();   //физические свойства
                    static_cast<CompPhysTemplate*>(compPhysTemplate2)->Deserialize(CompTemplateMgr::getInstance()->getRootTemplateNode(compName));
                    static_cast<CompPhysTemplate*>(compPhysTemplate2)->Deserialize(templ);
                    CompTemplateMgr::getInstance()->registerTemplate(compPhysTemplate2);

                }

            if (not (strcmp(templ->first_attribute("name")->value(),"CompVisualSq")))
                {
                    comp_id_type compName = comp_id_type("CompVisualSq");
                    CompVisualSqTemplate *compVisualSqTemplate2 = new CompVisualSqTemplate();   //внешний вид
                    static_cast<CompVisualSqTemplate*>(compVisualSqTemplate2)->Deserialize(CompTemplateMgr::getInstance()->getRootTemplateNode(compName));
                    static_cast<CompVisualSqTemplate*>(compVisualSqTemplate2)->Deserialize(templ);
                    CompTemplateMgr::getInstance()->registerTemplate(compVisualSqTemplate2);
                }

            if (not (strcmp(templ->first_attribute("name")->value(),"CompTogglable")))
                {
                    comp_id_type compName = comp_id_type("CompTogglable");
                    CompTogglableTemplate *CompTogglableTemplate2 = new CompTogglableTemplate();   //переключаемость
                    static_cast<CompTogglableTemplate*>(CompTogglableTemplate2)->Deserialize(CompTemplateMgr::getInstance()->getRootTemplateNode(compName));
                    static_cast<CompTogglableTemplate*>(CompTogglableTemplate2)->Deserialize(templ);

                    CompTemplateMgr::getInstance()->registerTemplate(CompTogglableTemplate2);

                }


            templ=templ->next_sibling("goc");
        }


        ob=ob->next_sibling("object_template");
    };


    //загружаем шаблоны игровых объектов

    ob = doc.first_node("object_template");

    while (ob)
    {
        comp_id_type objTemplName = comp_id_type(ob->first_attribute("name")->value());
        ObjectTemplate *objTemplate = new ObjectTemplate(objTemplName);
        cout<<"Loading template: \n";
        cout<<objTemplate->name()<<"\n";

        rapidxml::xml_node<char> *comp = ob->first_node("components")->first_node("goc");
        while (comp)
        {
            //static comp_id_type compName =  new comp_id_type;

            comp_id_type compName = comp_id_type(doc.allocate_string(comp->first_attribute("name")->value()));

            compName = comp->first_attribute("name")->value();

            //std::cout<<compName<< "\n";
            //std::cout<< static_cast<CompPhysTemplate*>(CompTemplateMgr::getInstance()->getTemplate(compName))->pos.first;
            objTemplate->addCompTemplate(CompTemplateMgr::getInstance()->getTemplate(compName));
            comp=comp->next_sibling("goc");
        };
        ObjTemplateMgr::getInstance()->registerTemplate( objTemplate );

        ob=ob->next_sibling("object_template");
    };



// создаем сами объекты

    ob = doc.first_node("object");

    while (ob)
    {

        comp_id_type name = comp_id_type (ob->first_attribute("name")->value());
        comp_id_type templ = comp_id_type (ob->first_attribute("template")->value());

        Object* obj = ObjTemplateMgr::getInstance()->createObject( templ, name);
        //cout << guest->getID();
        obj->Deserialize(ob);
        gameObjectsTable[obj->getID()]=obj;


       ob=ob->next_sibling("object");
    };

        std::cout << "gameObjectsTable.size() is " << (int) gameObjectsTable.size() << endl;


    //загружаем картинки


    Sprite* newSprite;
     ob = doc.first_node("sprite");
     while (ob)
     {

        newSprite = new Sprite;

        newSprite->bottom=atoi(ob->first_attribute("bottom")->value());
        newSprite->left=atoi(ob->first_attribute("left")->value());
        newSprite->right=atoi(ob->first_attribute("right")->value());
        newSprite->top=atoi(ob->first_attribute("top")->value());
        newSprite->textureHandle=texture;
        newSprite->texture_dimensions=texture_dimensions;

        spriteTable[atoi(ob->first_attribute("id")->value())]=newSprite;
        ob=ob->next_sibling("sprite");
     };
    registerEvents();

}

void Map::Finalise()
{


    //ws >> wt;
    std::ofstream mapfile;

    mapfile.open (".\\assets\\map.xml");

    //записываем стены

    std::set<vertex_tuple>::iterator i1;
	std::stringstream ws;
	std::string wt;
	for (i1=walls.begin();i1!=walls.end();i1++)
    {
        ws << i1->first << "," << i1->second << " ";
    }


    std::getline(ws,wt);


    rapidxml::xml_attribute<> *attr = doc.allocate_attribute("coords", wt.c_str());

    rapidxml::xml_node<char> *v;

    //rapidxml::xml_node<char> *v1=doc.allocate_node();

    v=doc.first_node("walls");

    rapidxml::xml_attribute<> *old_coords = v->first_attribute("coords");

    v->remove_attribute(old_coords);

    v->append_attribute(attr);


    //rapidxml::xml_node<> *g1 = gameObjectsTable["guest"]->Serialize(doc);
    for (std::map<obj_id_type,Object*>::iterator i = gameObjectsTable.begin();i!=gameObjectsTable.end();i++)
    {
        for (component_table_type::iterator i1 = i->second->Components()->begin();
        i1!=i->second->Components()->end(); i1++)
        {
            rapidxml::xml_node<>* g1 = i1->second->Serialize(doc);

            printf("serialized: \n");
            printf(g1->name());
            printf("\n");

            if (doc.first_node(g1->name()))
            {
                printf("found node \n");
                rapidxml::xml_node<char> *v1 = doc.first_node(g1->name());

                printf(v1->first_attribute("id")->value());
                printf("\n");
                printf(g1->first_attribute("id")->value());
                printf("\n");
                //printf("%i",strcmp(v1->first_attribute("id")->value(),g1->first_attribute("id")->value()));
                //printf("\n");

                if (not strcmp(v1->first_attribute("id")->value(),g1->first_attribute("id")->value()))
                {
                    doc.remove_node(v1);
                    doc.append_node(g1);
                    printf("replaced node \n");
                }
            };
        }
    };

    mapfile << doc;


    mapfile.close();

}


void Map::Update()
{
    int a=UpdateTimerSlot(Shell::GetElapsedTime());

    if (a)
    {
        printf("removeTask");
        UpdateTimerSlot.removeTask();
    }

    update_start = Shell::GetElapsedTime();

}

void Map::Render(int mode, int mode_element)
{

    glViewport(left,screen_height - min(width,height) - top,min(width,height),min(width,height));

    glMatrixMode (GL_MODELVIEW);

    glPushMatrix();

    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(mapLeft,mapRight,mapBottom,mapTop);


    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);

    glGetIntegerv(GL_VIEWPORT,viewport) ;



    glEnable(GL_TEXTURE_2D);

   	//glBindTexture(GL_TEXTURE_2D, (GLuint) texture);

    //гость

    for (std::map<obj_id_type,Object*>::iterator i = gameObjectsTable.begin();i!=gameObjectsTable.end();i++)
    {
	static_cast<CompVisualSq*>(i->second->getComponent("CompVisualSq"))->Draw();

	//static_cast<CompVisualSq*>(gameObjectsTable["Unknown"]->getComponent("CompVisualSq"))->Draw();
	//static_cast<CompVisualSq*>(gameObjectsTable["Unknown2"]->getComponent("CompVisualSq"))->Draw();
    }
    //координатные линии
	glColor4f(1.0,1.0,1.0,1.0);
	glBegin(GL_LINES);
        glVertex2f(-200.0f,0.0f);
        glVertex2f(200.0f,0.0f);
        glVertex2f(0.0f,200.0f);
        glVertex2f(0.0f,-200.0f);
	glEnd();
    //курсор
	glColor4f(1.0,1.0,1.0,1.0);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
        glVertex2f(cursorX,cursorY);
	glEnd();

    //середина экрана
	glColor4f(0.0,1.0,0.0,1.0);
	glBegin(GL_LINES);
        glVertex2f((mapRight+mapLeft)/2.0,(mapTop+mapBottom)/2.0);
        glVertex2f((mapRight+mapLeft)/2.0+(mapRight-mapLeft)*0.1,(mapTop+mapBottom)/2.0);
        glVertex2f((mapRight+mapLeft)/2.0,(mapTop+mapBottom)/2.0);
        glVertex2f((mapRight+mapLeft)/2.0,(mapTop+mapBottom)/2.0+(mapTop-mapBottom)*0.1);
	glEnd();


	glPopMatrix();




    glMatrixMode (GL_MODELVIEW);

	glPopMatrix();

	glViewport(0,0,screen_width,screen_height);
};

 float Map::LeastCostEstimate( void* nodeStart, void* nodeEnd )
	{
		short xStart, yStart, xEnd, yEnd;
		NodeToXY( nodeStart, &xStart, &yStart );
		NodeToXY( nodeEnd, &xEnd, &yEnd );


		/* Compute the minimum path cost using distance measurement. It is possible
		   to compute the exact minimum path using the fact that you can move only
		   on a straight line or on a diagonal, and this will yield a better result.
		*/
		 int dx = xStart - xEnd;
		int dy = yStart - yEnd;
		return (float) sqrt((double)(dx*dx) + (double)(dy*dy)) ;

	}

 void Map::AdjacentCost( void* node, std::vector< micropather::StateCost > *neighbors )
	{

		short x, y;
		const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
		const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
		const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

		NodeToXY( node, &x, &y );

		for( int i=0; i<8; ++i ) {
			int nx = x + dx[i];
			int ny = y + dy[i];
			//printf("locating neighbours \n");

			int pass = Passable( nx, ny );

			if ( pass > 0 ) {
				if ( pass == 1 )
				{
					// Normal floor
					micropather::StateCost nodeCost = { XYToNode( nx, ny ), cost[i] };
					neighbors->push_back( nodeCost );
				}
				else
				{
					// Normal floor
					//micropather::StateCost nodeCost = { XYToNode( nx, ny ), FLT_MAX };
					//neighbors->push_back( nodeCost );
				}
			}
		}

	}

 void Map::PrintStateInfo( void* node )
	{
		short x, y;
		NodeToXY( node, &x, &y );
		printf( "(%i,%i)", x, y );
	}

 void Map::NodeToXY( void* node, short* x, short* y )
	{
		//int index = (int)node;

		unsigned int si = (unsigned int) node;

		*y = si & 0xffff;
        *x = (si >> 16) & 0xffff;

	}

 void* Map::XYToNode( short x, short y )
    {

    unsigned short s1 = (unsigned short) x; //s1 ranges from 0 to 65535
    unsigned short s2 = (unsigned short) y;
    unsigned int s =(s1<<16)+s2;

    return (void*)s;

	//return (void*) ( (y-minY)*gameMapX + (x-minX) );
	}

int Map::Passable( short nx, short ny )
	{
		if (walls.find(vertex_tuple(nx,ny))!=walls.end())
		{
		    return 0;
		}

		return 1;
	}

ComplexTask* Map::planPath(short x, short y)
{
    micropather::MicroPather pather( this );

    std::vector< void* > path;
	float totalCost;

	vertex_tuple tpos = static_cast<CompPhys*>(gameObjectsTable["Guest"]->getComponent("CompPhys"))->pos;

    int result = pather.Solve( XYToNode(tpos.first,tpos.second),XYToNode(x,y), &path, &totalCost );

    if (result)
    {
        return NULL;
    };


    static goToPoint orderChain[256];

    short x1, y1;

    NodeToXY( path[0], &x1, &y1 );
    orderChain[0]=*(new goToPoint(x1,y1,gameObjectsTable["Guest"]));
    static ComplexTask followPath;
    followPath=ComplexTask(orderChain[0]);
    for (int i =1;i<path.size();i++){

        NodeToXY( path[i], &x1, &y1 );

        orderChain[i-1]=*(new goToPoint(x1,y1,gameObjectsTable["Guest"]));
        followPath.AddAction(orderChain[i-1]);
    }
    return &followPath;

}



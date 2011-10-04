
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


#ifdef __MY_NOT__
#define not !
#endif

// The game's element context (declared in main.cpp).
extern Rocket::Core::Context* context;
#define MAX_POINTS 3
struct map_element_info{
	int dx[MAX_POINTS];
	int dy[MAX_POINTS];
	int npoints;
	//float dy1, dy2, dy3, dy4;
	GLfloat r,g,b;
};
map_element_info tube_info={{0},{0},1,1.0, 1.0, 1.0,};
map_element_info valve_info={{0},{0},1, 1.0, 1.0, 0};
map_element_info walls_info={{0},{0},1, 0, 0, 0};
map_element_info res_gen_start_info={{0},{0},1, 0.0, 0.0, 1.0};
map_element_info gen_info={{0,1},{0,0},2, 0.0, 1.0, 0.0};
map_element_info water_gen_info={{0,0,0},{0,1,2},3, 0.0, 1.0, 1.0};
map_element_info control_info={{0},{0},1, 1.0, 0.0, 0.0};
map_element_info pump_info={{0,0},{0,1},2, 1.0, 0.0, 1.0};
map_element_info light_info={{0},{0},1, 1.0, 1.0, 0.0};
map_element_info outer_link_info={{0},{0},1, 1.0, 1.0, 1.0};
map_element_info patient_info={{0},{0},1, 1.0, 1.0, 1.0};
map_element_info alarm_magistral_info={{0},{0},1,0.0, 0.0, 0.0};
map_element_info reserve_magistral_info={{0},{0},1, 0.0, 0.0, 1.0};
map_element_info div_corb_info={{0},{0},1, 0.0, 1.0, 0.0};
map_element_info corb_info={{0},{0},1, 1.0, 0.0, 0.0};

Map::Map()
{


	Rocket::Core::GetRenderInterface()->LoadTexture(texture, texture_dimensions, "assets/invader.tga");
	if_res_gen_start=false;
	if_gen=false;
	if_water_gen=false;
	if_control=false;
	if_pump=false;
	if_light=false;
	if_outer_link=false;
	if_patient=false;
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


    pc1.pos.first=5;

    pc1.pos.second=5;

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



    //загружаем проводку

    //powerGrid.reserve(90);
	powerGrid.resize(90);

    rapidxml::xml_node<char> *n1=doc.first_node("powerGrid");

    n1=n1->first_node("powerGridNode");

    list < rapidxml::xml_node <char>* > q;
    q.push_back(n1);


    while( !q.empty( ) ){
            v = q.front();
            q.pop_front();      // remove it from the list

            powerGridNode *pn=new powerGridNode;

            printf("powerGridNode: \n");

            short curId;
            if (v->first_attribute("id")){
                curId=atoi(v->first_attribute("id")->value());
            } else {
                curId=87;               //TODO:придумать раздачу незанятых номеров
            }
            pn->objectId=curId;

            printf("id: \n");

            printf("%i \n",curId);

            std::string coords = v->first_attribute("pos")->value();  //координаты
            std::vector<std::string> pairs = split(coords,',');

            pn->pos.first=atoi(pairs[0].c_str());
            pn->pos.second=atoi(pairs[1].c_str());

            printf("coords: \n");

            printf("%i \n",pn->pos.first);
            printf("%i \n",pn->pos.second);

            if (v->first_attribute("children")){
                std::string c1 = v->first_attribute("children")->value();
                std::vector<std::string> children = split(c1,',');

                printf("children: \n");

                for (std::vector<std::string>::iterator p1 = children.begin(); p1!=children.end();p1++)
                {
                    pn->children.push_back(atoi(p1->c_str()));
                    printf("%i \n",(atoi(p1->c_str())));
                }
            }
            powerGrid[pn->objectId]=pn;

            if (v->next_sibling("powerGridNode"))
            {
                q.push_back((v->next_sibling("powerGridNode")));
            }

    }

    //загружаем игровые объекты

    rapidxml::xml_node<char> *ob;

    if (doc.first_node("guest"))
    {
        ob = doc.first_node("guest");

        //int i1 = getUniqueId();
        int i1 = 1;
        //gameObjectsTable.insert(std::pair<int,Entity*>(i1, new Guest));


        gameObjectsTable[i1]=new Guest;
        gameObjectsTable[i1]->Deserialize(ob);



        std::cout << "gameObjectsTable.size() is " << (int) gameObjectsTable.size() << endl;

        for (std::map<int,Entity*>::iterator go1=gameObjectsTable.begin();go1!=gameObjectsTable.end();go1++)
            {
                printf("guest pos x: \n");
                printf("% i \n",go1->second->pos.first);
                printf("guest pos y: \n");
                printf("% i \n",go1->second->pos.second);
            };


    };

    //загружаем картинки

     ob = doc.first_node("sprite");
     while (ob!=0)
     {

        //int i1 = getUniqueId();
        //gameObjectsTable.insert(std::pair<int,Entity*>(i1, new Guest));

        static Sprite newSprite;
        newSprite.bottom=atoi(ob->first_attribute("bottom")->value());
        newSprite.left=atoi(ob->first_attribute("left")->value());
        newSprite.right=atoi(ob->first_attribute("right")->value());
        newSprite.top=atoi(ob->first_attribute("top")->value());
        newSprite.textureHandle=texture;
        newSprite.texture_dimensions=texture_dimensions;

        spriteTable[atoi(ob->first_attribute("id")->value())]=&newSprite;
        ob=ob->next_sibling("sprite");
     };


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


    rapidxml::xml_node<> *g1 = gameObjectsTable[1]->Serialize(doc);

    printf("serialized id: \n");
    printf(g1->name());
    printf("\n");
    printf(g1->first_attribute("id")->value());
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



    //записываем проводку

    //v=doc.first_node("powerGrid");
        //v->remove_all_nodes


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

void RenderElements(std::set<vertex_tuple> &elements,
						 map_element_info& info){
	glColor3f(info.r, info.g, info.b);
	glBegin(GL_QUADS);

	for (std::set<vertex_tuple>::iterator i1=elements.begin();i1!=elements.end();i1++)
	{
		/*glVertex2f(i1->first+info.dx1,i1->second+info.dy1);
		glVertex2f(i1->first+info.dx2,i1->second+info.dy2);
		glVertex2f(i1->first+info.dx3,i1->second+info.dy3);
		glVertex2f(i1->first+info.dx4,i1->second+info.dy4);*/
		for(int j=0;j<info.npoints;j++){
			glVertex2f(i1->first+info.dx[j]-0.5,i1->second+info.dy[j]-0.5);
			glVertex2f(i1->first+info.dx[j]-0.5,i1->second+info.dy[j]+0.5);
			glVertex2f(i1->first+info.dx[j]+0.5,i1->second+info.dy[j]+0.5);
			glVertex2f(i1->first+info.dx[j]+0.5,i1->second+info.dy[j]-0.5);
		}
	}
	glEnd();
}
void RenderElement(vertex_tuple &element, map_element_info&info, bool if_render){
	if(!if_render)
		return;
	glColor3f(info.r, info.g, info.b);
	glBegin(GL_QUADS);

	for(int j=0;j<info.npoints;j++){
		glVertex2f(element.first+info.dx[j]-0.5,element.second+info.dy[j]-0.5);
		glVertex2f(element.first+info.dx[j]-0.5,element.second+info.dy[j]+0.5);
		glVertex2f(element.first+info.dx[j]+0.5,element.second+info.dy[j]+0.5);
		glVertex2f(element.first+info.dx[j]+0.5,element.second+info.dy[j]-0.5);
	}
	glEnd();

}
void RenderCursor(int cursorX, int cursorY, map_element_info &info){
    glColor3f(info.r, info.g, info.b);
	glBegin(GL_QUADS);
	for(int j=0;j<info.npoints;j++){
		glVertex2f(cursorX+info.dx[j]-0.5,cursorY+info.dy[j]-0.5);
		glVertex2f(cursorX+info.dx[j]-0.5,cursorY+info.dy[j]+0.5);
		glVertex2f(cursorX+info.dx[j]+0.5,cursorY+info.dy[j]+0.5);
		glVertex2f(cursorX+info.dx[j]+0.5,cursorY+info.dy[j]-0.5);
	}
    glEnd();
}
void Map::Render(int mode, int mode_element)
{

    glViewport(left,screen_height - height - top,width,height);

    glMatrixMode (GL_MODELVIEW);

    glPushMatrix();

    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(-5,5,-5,5);


    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);

    glGetIntegerv(GL_VIEWPORT,viewport) ;



    glEnable(GL_TEXTURE_2D);

   	//glBindTexture(GL_TEXTURE_2D, (GLuint) texture);

    //гость
	gameObjectsTable[1]->Draw();

	ps1.Draw();

	pc1.Draw();




    //координатные линии
	glColor4f(1.0,1.0,1.0,1.0);
	glBegin(GL_LINES);
        glVertex2f(-200.0f,0.0f);
        glVertex2f(200.0f,0.0f);
        glVertex2f(0.0f,200.0f);
        glVertex2f(0.0f,-200.0f);
	glEnd();
	if(mode==MAP){
    //стены
		RenderElements(walls, walls_info);
		RenderElement(res_gen_start, res_gen_start_info, if_res_gen_start);
		RenderElement(gen, gen_info, if_gen);
		RenderElement(water_gen, water_gen_info, if_water_gen);
		RenderElement(control, control_info, if_control);
		RenderElement(pump, pump_info, if_pump);
		RenderElement(light, light_info, if_light);
		RenderElement(outer_link, outer_link_info, if_outer_link);
		RenderElement(patient, patient_info, if_patient);
		if(mode_element==WALL){
			RenderCursor(cursorX, cursorY, walls_info);
		}
		else if(mode_element==RES_GEN_START){
			RenderCursor(cursorX, cursorY, res_gen_start_info);
		}
		else if(mode_element==GEN){
			RenderCursor(cursorX, cursorY, gen_info);
		}
		else if(mode_element==WATER_GEN){
			RenderCursor(cursorX, cursorY, water_gen_info);
		}
		else if(mode_element==CONTROL){
			RenderCursor(cursorX, cursorY, control_info);
		}
		else if(mode_element==PUMP){
			RenderCursor(cursorX, cursorY, pump_info);
		}
		else if(mode_element==LIGHT){
			RenderCursor(cursorX, cursorY, light_info);
		}
		else if(mode_element==OUTER_LINK){
			RenderCursor(cursorX, cursorY, outer_link_info);
		}
		else if(mode_element==PATIENT){
			RenderCursor(cursorX, cursorY, patient_info);
		}

    /*glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);

	for (std::set<vertex_tuple>::iterator i1=walls.begin();i1!=walls.end();i1++)
    {
        glVertex2f(i1->first-0.5,i1->second-0.5);
        glVertex2f(i1->first-0.5,i1->second+0.5);
        glVertex2f(i1->first+0.5,i1->second+0.5);
        glVertex2f(i1->first+0.5,i1->second-0.5);
    }
	glEnd();*/
	}else if(mode==CANALISATION){
		RenderElements(tubes, tube_info);
		RenderElements(valves, valve_info);
		if(mode_element==TUBE){
			RenderCursor(cursorX, cursorY, tube_info);
		}
		else if(mode_element==VALVE){
			RenderCursor(cursorX, cursorY, valve_info);
		}
	}else if(mode==ELECTRICITY){
		RenderElements(alarm_magistral, alarm_magistral_info);
		RenderElements(reserve_magistral, reserve_magistral_info);
		RenderElements(div_corb, div_corb_info);
		RenderElements(corb, corb_info);
		if(mode_element==ALARM_MAGISTRAL){
			RenderCursor(cursorX, cursorY, alarm_magistral_info);
		}
		else if(mode_element==RESERVE_MAGISTRAL){
			RenderCursor(cursorX, cursorY, reserve_magistral_info);
		}
		else if(mode_element==DIV_CORB){
			RenderCursor(cursorX, cursorY, div_corb_info);
		}
		else if(mode_element==CORB){
			RenderCursor(cursorX, cursorY, corb_info);
		}
	}

	//проводка

    /*glColor3f(0.0,1.0,0.0);
    set <powerGridNode*> q;
    set <powerGridNode*> visited;
    powerGridNode* v=powerGrid[0];

    q.insert(v);
    visited.insert(v);
    while( !q.empty( ) ){
            v = *(q.begin());
            q.erase(q.begin());
            for (std::vector<short>::iterator i1=v->children.begin();i1!=v->children.end();i1++)
            {
                glBegin(GL_LINES);
                    glVertex2f((v)->pos.first,(v)->pos.second);
                    glVertex2f((powerGrid[*i1])->pos.first,(powerGrid[*i1])->pos.second);
                glEnd();
                if (visited.find(powerGrid[*i1])==visited.end())
                {
                    q.insert(powerGrid[*i1]);
                    visited.insert(powerGrid[*i1]);
                }
            }
    }*/


    //курсор
    /*glColor3f(1,1,0);
	glBegin(GL_QUADS);
        glVertex2f(cursorX-0.5,cursorY-0.5);
        glVertex2f(cursorX-0.5,cursorY+0.5);
        glVertex2f(cursorX+0.5,cursorY+0.5);
        glVertex2f(cursorX+0.5,cursorY-0.5);
    glEnd();*/


	glPopMatrix();

    glMatrixMode (GL_MODELVIEW);

	glPopMatrix();

	glViewport(0,0,screen_width,screen_height);




};
bool find_and_delete_element(std::set<vertex_tuple>&elements, vertex_tuple &v1){
	return elements.erase(v1);
}
bool search_element(vertex_tuple &element, map_element_info &element_info, vertex_tuple &v1, map_element_info &v1_info){
	for(int i=0;i<element_info.npoints;i++){
		for(int j=0;j<v1_info.npoints;j++){
			if((element.first+element_info.dx[i]==v1.first+v1_info.dx[j]) &&
				(element.second+element_info.dy[i]==v1.second+v1_info.dy[j]))
				return true;
		}
	}
	return false;
}
bool search_elements(std::set<vertex_tuple>&elements, map_element_info &elements_info, 
					 vertex_tuple &v1, map_element_info &v1_info){
	for(std::set<vertex_tuple>::iterator i=elements.begin();i!=elements.end();i++){
		if(search_element(*i, elements_info, v1, v1_info))
			return true;
	}
	return false;
}

void Map::try_insert_element(vertex_tuple &v1,int mode, int mode_element){
	if(mode==CANALISATION){
		map_element_info *v1_info=NULL;
		std::set<vertex_tuple> *target_set=NULL;
		if(mode_element==TUBE){
			v1_info=&tube_info;
			target_set=&tubes;
		}
		else if(mode_element==VALVE){
			v1_info=&valve_info;
			target_set=&valves;
		}
		if(search_elements(tubes, tube_info, v1, *v1_info))
			return;
		if(search_elements(valves, valve_info, v1, *v1_info))
			return;
		target_set->insert(v1);
	}else if(mode==MAP){
		map_element_info *v1_info=NULL;
		if(mode_element==WALL){
			v1_info=&walls_info;
		}else if(mode_element==RES_GEN_START){
			v1_info=&res_gen_start_info;
		}else if(mode_element==GEN){
			v1_info=&gen_info;
		}else if(mode_element==WATER_GEN){
			v1_info=&water_gen_info;
		}else if(mode_element==CONTROL){
			v1_info=&control_info;
		}else if(mode_element==PUMP){
			v1_info=&pump_info;
		}else if(mode_element==LIGHT){
			v1_info=&light_info;
		}else if(mode_element==OUTER_LINK){
			v1_info=&outer_link_info;
		}else if(mode_element==PATIENT){
			v1_info=&patient_info;
		}
		if(search_elements(walls, walls_info, v1, *v1_info))
			return;
		if(if_res_gen_start && search_element(res_gen_start, res_gen_start_info, v1, *v1_info))
			return;
		if(if_gen && search_element(gen, gen_info, v1, *v1_info))
			return;
		if(if_water_gen && search_element(water_gen, water_gen_info, v1, *v1_info))
			return;
		if(if_control && search_element(control, control_info, v1, *v1_info))
			return;
		if(if_pump && search_element(pump, pump_info, v1, *v1_info))
			return;
		if(if_light && search_element(light, light_info, v1, *v1_info))
			return;
		if(if_outer_link && search_element(outer_link, outer_link_info, v1, *v1_info))
			return;
		if(if_patient && search_element(patient, patient_info, v1, *v1_info))
			return;

		if(mode_element==WALL){
			walls.insert(v1);
		}else if(mode_element==RES_GEN_START){
			res_gen_start=v1;
			if_res_gen_start=true;
		}else if(mode_element==GEN){
			gen=v1;
			if_gen=true;
		}else if(mode_element==WATER_GEN){
			water_gen=v1;
			if_water_gen=true;
		}else if(mode_element==CONTROL){
			control=v1;
			if_control=true;
		}else if(mode_element==PUMP){
			pump=v1;
			if_pump=true;
		}else if(mode_element==LIGHT){
			light=v1;
			if_light=true;
		}else if(mode_element==OUTER_LINK){
			outer_link=v1;
			if_outer_link=true;
		}else if(mode_element==PATIENT){
			patient=v1;
			if_patient=true;
		}

	}else if(mode==ELECTRICITY){
		map_element_info *v1_info=NULL;
		std::set<vertex_tuple> *target_set=NULL;
		if(mode_element==ALARM_MAGISTRAL){
			v1_info=&alarm_magistral_info;
			target_set=&alarm_magistral;
		}
		else if(mode_element==RESERVE_MAGISTRAL){
			v1_info=&reserve_magistral_info;
			target_set=&reserve_magistral;
		}else if(mode_element==DIV_CORB){
			v1_info=&div_corb_info;
			target_set=&div_corb;
		}else if(mode_element==CORB){
			v1_info=&corb_info;
			target_set=&corb;
		}

		if(search_elements(alarm_magistral, alarm_magistral_info, v1, *v1_info))
			return;
		if(search_elements(reserve_magistral, reserve_magistral_info, v1, *v1_info))
			return;
		if(search_elements(div_corb, div_corb_info, v1, *v1_info))
			return;
		if(search_elements(corb, corb_info, v1, *v1_info))
			return;
		target_set->insert(v1);
	}
}
void Map::try_delete_element(vertex_tuple &v1, int mode){
	if(mode==CANALISATION){
		if(find_and_delete_element(tubes, v1))
			;
		else if(find_and_delete_element(valves, v1))
			;
	}else if(mode==MAP){
		if(find_and_delete_element(walls, v1))
			;
		else if(if_res_gen_start && res_gen_start==v1){
			if_res_gen_start=false;
		}else if(if_gen && gen==v1){
			if_gen=false;
		}else if(if_water_gen && water_gen==v1){
			if_water_gen=false;
		}else if(if_control && control==v1){
			if_control=false;
		}else if(if_pump && pump==v1){
			if_pump=false;
		}else if(if_light && light==v1){
			if_light=false;
		}else if(if_outer_link && outer_link==v1){
			if_outer_link=false;
		}else if(if_patient && patient==v1){
			if_patient=false;
		}
	}else if(mode==ELECTRICITY){
		if(find_and_delete_element(alarm_magistral, v1))
			;
		else if(find_and_delete_element(reserve_magistral, v1))
			;
		else if(find_and_delete_element(div_corb, v1))
			;
		else if(find_and_delete_element(corb, v1))
			;
	}
}
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

    int result = pather.Solve( XYToNode(gameObjectsTable[1]->pos.first,gameObjectsTable[1]->pos.second),XYToNode(x,y), &path, &totalCost );

    if (result)
    {
        return NULL;
    };


    static goToPoint orderChain[256];

    short x1, y1;

    NodeToXY( path[0], &x1, &y1 );
    orderChain[0]=*(new goToPoint(x1,y1,gameObjectsTable[1]));
    static ComplexTask followPath;
    followPath=ComplexTask(orderChain[0]);
    for (int i =1;i<path.size();i++){

        NodeToXY( path[i], &x1, &y1 );

        orderChain[i-1]=*(new goToPoint(x1,y1,gameObjectsTable[1]));
        followPath.AddAction(orderChain[i-1]);
    }
    return &followPath;

}



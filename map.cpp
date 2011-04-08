

#undef ROCKET_PLATFORM_WIN32
#undef __WIN32__
#undef _WIN32

#include "map.hpp"
#include <Rocket/Core.h>
#include <cmath>
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

    update_start=0;
	// How often the invaders move
	update_freq = 0.2;


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
    v=doc.first_node("walls");
    std::string coords = v->first_attribute("coords")->value();
    std::vector<std::string> pairs = split(coords,' ');


    /*
    int minX,maxX,minY,maxY;
    minX=0;
    minY=0;
    maxX=0;
    maxY=0;
    */
    for (std::vector<std::string>::iterator p1 = pairs.begin(); p1!=pairs.end();p1++)
    {
        std::vector<std::string> xy = split(*p1,',');

        vertex_tuple v1;

        v1.first=atoi(xy[0].c_str());
        v1.second=atoi(xy[1].c_str());
        walls.insert(v1);
    }

    micropather::MicroPather pather( this );

	std::vector< void* > path;
	float totalCost;

	void* startNode;
	void* endNode;
    startNode = XYToNode( -50, 0 );
    endNode = XYToNode( 105, -35 );

	std::cout << "\n";
	PrintStateInfo(startNode);
    std::cout << "\n";
    PrintStateInfo(endNode);
    std::cout << "\n";



	int result = pather.Solve( startNode,endNode, &path, &totalCost );


	std::cout << "result:" << result << "totalcost" << totalCost<<"\n";

	for (std::vector< void* >::iterator p1=path.begin();p1!=path.end();p1++)
	{
        short x, y;
        NodeToXY( *p1, &x, &y );
        std::cout << x << "," <<y <<"\n";
	};

	if (UpdateTimerSlot.m_Observers.size()==0){
        static ComplexTask* followPath;
        followPath = planPath(10,10);
        if (followPath!=NULL)
        {
            UpdateTimerSlot.addTask<ComplexTask>(*followPath);
        }
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
    int a=UpdateTimerSlot(Shell::GetElapsedTime());

    if (a)
    {
        printf("removeTask");
        UpdateTimerSlot.removeTask();
    }

    update_start = Shell::GetElapsedTime();

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

	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
        glVertex2f(-200.0f,0.0f);
        glVertex2f(200.0f,0.0f);
        glVertex2f(0.0f,200.0f);
        glVertex2f(0.0f,-200.0f);
	glEnd();

    glColor3f(1.0,1.0,1.0);
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

	guest1.Draw();

    glColor3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
        glVertex2f(cursorX-0.5,cursorY-0.5);
        glVertex2f(cursorX-0.5,cursorY+0.5);
        glVertex2f(cursorX+0.5,cursorY+0.5);
        glVertex2f(cursorX+0.5,cursorY-0.5);
    glEnd();



	glPopMatrix();

    glMatrixMode (GL_MODELVIEW);

	glPopMatrix();

	glViewport(0,0,1024,768);

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

    int result = pather.Solve( XYToNode(guest1.pos.first,guest1.pos.second),XYToNode(x,y), &path, &totalCost );

    if (result)
    {
        return NULL;
    };


    static goToPoint orderChain[256];

    short x1, y1;

    NodeToXY( path[0], &x1, &y1 );
    orderChain[0]=*(new goToPoint(x1,y1,&guest1));
    static ComplexTask followPath;
    followPath=ComplexTask(orderChain[0]);
    for (int i =1;i<path.size();i++){

        NodeToXY( path[i], &x1, &y1 );

        orderChain[i-1]=*(new goToPoint(x1,y1,&guest1));
        followPath.AddAction(orderChain[i-1]);
    }
    return &followPath;

}



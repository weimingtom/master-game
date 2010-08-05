#include "geomap.hpp"
#include "globals.hpp"
#include "pcre/pcre.h"



VisiLibity::Visibility_Polygon visiPoly;
Polygon_with_holes_2 visiBounded;


GLUtesselator *tess;

/*
template <typename PARAM>
Uint32 timerFunc(void *obj, Uint32 ival, void *arg)
{
    static Slot<PARAM> b1=*(Slot<PARAM>*)arg;
    b1(ival);
    return 1;
};
*/

GLvoid CALLBACK tcbBegin (GLenum prim)
{
   glBegin (prim);
}

GLvoid CALLBACK tcbVertex (void *data)
{
   glVertex3dv ((GLdouble *)data);
}

GLvoid CALLBACK tcbEnd ()
{
   glEnd ();
}


geoData mapload(char* path,VisiLibity::Environment & mapEnv,VisiLibity::Visibility_Graph & visGraph,float clearDist )
{


    std::ifstream in(path);
    std::string s;
    std::string s1="";

    while(getline(in, s)) { // Discards newline char
    s1=s1+s+"\n";
    }

    std::vector<char> xml_copy(s1.begin(), s1.end());
    xml_copy.push_back('\0');

    rapidxml::xml_document<> doc;    // character type defaults to char
    doc.parse<0>(&xml_copy[0]);    // 0 means default parse flags

    //std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

    rapidxml::xml_node<char> *n1;

    n1=doc.first_node("svg");
    n1=n1->first_node("g")->first_node("path");

    //std::vector < VisiLibity::Point > poly_temp;

    //geoData vertices_temp(10,poly_temp);
    geoData vertices_temp;

    int i=0;

    while (n1)
    {
        vertices_temp.push_back(loadPath(n1->first_attribute("d")->value()));
        n1=n1->next_sibling("path");
        i++;
    }


   Polygon_2 mapEnvOB;

    for (int j=0;j<vertices_temp[0].size();j++)
        {
            Point_2 nextVert= Point_2(vertices_temp[0][j][0],vertices_temp[0][j][1]);
            mapEnvOB.push_back(nextVert);
        };


        Polygon_set_2 S;
    mapEnvOB.reverse_orientation();

        S.insert(mapEnvOB);



    for (int k=1;k<vertices_temp.size();k++)
    {
        Polygon_2 holeCGAL;
        for (int j=0;j<vertices_temp[k].size();j++)
        {
            holeCGAL.push_back(Point_2(vertices_temp[k][j][0],vertices_temp[k][j][1]));
        };

        //holeCGAL.reverse_orientation();

        S.difference(holeCGAL);

        holeCGAL.clear();
    };

  std::list<Polygon_with_holes_2> res;
  std::list<Polygon_with_holes_2>::const_iterator it;


   S.polygons_with_holes (std::back_inserter (res));

    std::vector<VisiLibity::Polygon> envPolys;

   for (it = res.begin(); it != res.end(); ++it)
   {
      if(CGAL::ON_BOUNDED_SIDE==CGAL::bounded_side_2(it->outer_boundary().vertices_begin(),it->outer_boundary().vertices_end(),Point_2(guest1.pos.x(),guest1.pos.y()),Kernel()))
      {
        envPolys.push_back(ConvertPolygonCGAL2Vis(it->outer_boundary()));

        Polygon_with_holes_2::Hole_const_iterator hi;
        for (hi=it->holes_begin();hi!=it->holes_end();++hi)
        {
            envPolys.push_back(ConvertPolygonCGAL2Vis(*hi));
        };

        break;
      };

  }


    for (int i=0;i<envPolys.size();i++){
        //envPolys.push_back(VisiLibity::Polygon(vertices_temp[i]));
        //i=0;
        envPolys[i].eliminate_redundant_vertices(0.0001);
        VisiLibity::Point cm=envPolys[i].centroid();
            for (int j=0;j<envPolys[i].n();j++)
            {
                    if (j<envPolys[i].n()-1){
                        VisiLibity::Point n1=clearDist*normal(envPolys[i][j+1]-envPolys[i][j]);
                        envPolys[i][j]=envPolys[i][j]+n1;
                        envPolys[i][j+1]=envPolys[i][j+1]+n1;
                    }
            }
            VisiLibity::Point norm1=clearDist*normal(envPolys[i][0]-envPolys[i][envPolys[i].n()-1]);
            envPolys[i][0]=envPolys[i][0]+norm1;
            envPolys[i][envPolys[i].n()-1]=envPolys[i][envPolys[i].n()-1]+norm1;
    };

    mapEnv = *(new VisiLibity::Environment(envPolys));
    mapEnv.enforce_standard_form();

    visGraph = *(new VisiLibity::Visibility_Graph(mapEnv,0.00000001));
    return vertices_temp;

};

void mapdraw()
{


    VisiLibity::Point pt1;
    //pt1=VisiLibity::Point(160.0f,180.0f);
    pt1=guest1.pos;
    pt1.snap_to_boundary_of(mapEnv);
    pt1.snap_to_vertices_of(mapEnv);

    visiPoly=VisiLibity::Visibility_Polygon(pt1,mapEnv,0.00001);

    Polygon_2 CGALvp;
    for (int j=0;j<visiPoly.n();j++){
            CGALvp.push_back(Point_2(visiPoly[j].x(),visiPoly[j].y()));
        };

 //   CGAL::difference(mapEnvOB,holeCGAL,std::back_inserter(mapEnvOB));


    Polygon_2 visRange=ngon(20,150.0);
    Pwh_list_2 intR;
    CGAL::intersection(CGALvp,visRange,std::back_inserter(intR));
    visiBounded=intR.front();

    tess = gluNewTess();
    gluTessCallback (tess, GLU_TESS_BEGIN, (GLvoid (CALLBACK *)())tcbBegin);
    gluTessCallback (tess, GLU_TESS_VERTEX, (GLvoid (CALLBACK *)())tcbVertex);
    gluTessCallback (tess, GLU_TESS_END, (GLvoid (CALLBACK *)())tcbEnd);

};

void MapDrawFunction(AG_Event *event)
{
    GLdouble vz = -2.0;
    GLfloat ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat diffuse[4] = { 1.f, 1.0f, 1.0f, 1.0f };
    GLfloat specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    int wireframe = 0;
    enum { FLATSHADING, SMOOTHSHADING } shading = FLATSHADING;

    int i;
	GLfloat pos[4];

	glLoadIdentity();
	glPushAttrib(GL_POLYGON_BIT|GL_LIGHTING_BIT|GL_DEPTH_BUFFER_BIT);

    glEnable(GL_POINT_SMOOTH);

    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

// Create light components
    //GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 0.1f };
    GLfloat diffuseLight[] = { 1, 1, 1, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { cursorwX, cursorwY, 0.5f, 1.0f };

// Assign created components to GL_LIGHT0
    //glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.002f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

    glClearColor(0.0f,0.0f,0.0f,1.0f);

	glPushMatrix();
	glTranslated(0.0, 0.0, vz);
    GLfloat visi[4]={1.0,0.0,0.0,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, visi);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, visi);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, visi);




    glColor3f(0.9,0.9,0.6);

    mapdraw();

    for (int i=0;i<sceneVertices.size();i++){
        glBegin(GL_LINE_LOOP);
        for (int j=0;j<sceneVertices[i].size();j++){
            glVertex3f( sceneVertices[i][j][0], sceneVertices[i][j][1], 0.0f);
        };
        glEnd();
    };



    glColor3f(0.9,0.9,0.0);
    for (int i=0;i<mapEnvCollision.h()+1;i++){
        glBegin(GL_LINE_LOOP);
        for (int j=0;j<mapEnvCollision[i].n();j++){
            glVertex3f( mapEnvCollision[i][j].x(), mapEnvCollision[i][j].y(), 0.0f);
        };
        //glVertex3f( sceneVertices[i][0].x(), sceneVertices[i][0].y(), 0.0f);
        glEnd();
    };



    gluTessBeginPolygon (tess, NULL);
    gluTessBeginContour (tess);

    GLdouble data[visiBounded.outer_boundary().size()][3];

    CGAL::Polygon_2<Kernel>::Vertex_iterator  vit;
    i=0;
    for  (vit=visiBounded.outer_boundary().vertices_begin();vit!=visiBounded.outer_boundary().vertices_end();++vit)
    {
        data[i][0]=vit->x();
        data[i][1]=vit->y();
        data[i][2]=0;
        gluTessVertex (tess, data[i], data[i]);
        i++;
    }
    /*
    for (i=0; i<visiPoly.n();i++)
    {
        data[i][0]=visiPoly[i].x();
        data[i][1]=visiPoly[i].y();
        data[i][2]=0;
        gluTessVertex (tess, data[i], data[i]);
    }
    */
    gluTessEndContour (tess);
    gluEndPolygon (tess);

    glBegin(GL_LINE_LOOP);
        for (int i=0;i<visiPoly.n();i++){
            glVertex3f(visiPoly[i].x(),visiPoly[i].y(),0.0f);
        }
    glEnd();

    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_STRIP);
        for (int i=0;i<motionPath.size();i++){
            glVertex3f(motionPath[i].x(),motionPath[i].y(),0.1f);
        }
    glEnd();


    //glPopMatrix();
    GLfloat marker[4]={1.0,0.0,0.0,1.0};
    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, marker);
    glColor3f(1.0,0.0,0.0);
    guest1.Draw();


    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);
    glGetIntegerv(GL_VIEWPORT,viewport) ;


    glColor3f(1.0,1.0,1.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(cursorwX,cursorwY+1/sqrt(2)*10,1.0f);
        glVertex3f(cursorwX-1/sqrt(2)*10,cursorwY-1/sqrt(2)*10,1.0f);
        glVertex3f(cursorwX+1/sqrt(2)*10,cursorwY-1/sqrt(2)*10,1.0f);
    glEnd();


    glPopMatrix();
    glPopAttrib();

    };
void
MapScaleFunction(AG_Event *event)
{
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(mapXMin, mapXMax, mapYMin, mapYMax, 0.1, 100.0);

	glOrtho(mapCenterX-mapWidth/2, mapCenterX+mapWidth/2, mapCenterY+mapHeight/2,mapCenterY-mapHeight/2, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
};

void
MapMoveFunction(AG_Event *event)
{
        void *p=AG_SELF();
    int cursorX=AG_INT(1);
    int cursorY=AG_INT(2);

};

void
MapClickFunction(AG_Event *event)
{
    void *p=AG_SELF();
    int b=AG_INT(1);
    int cursorX=AG_INT(2);
    int cursorY=AG_INT(3);

    GLdouble posX,posY,posZ;

    gluUnProject(viewport[0]+cursorX,viewport[1] +viewport[3]-cursorY,0,modelview,projection,viewport,&posX,&posY,&posZ);

    cursorwX=posX;
    cursorwY=posY;

    static ComplexTask followPath;
    followPath = planPath(cursorwX,cursorwY);
    if (UpdateTimerSlot.m_Observers.size()==0)
        UpdateTimerSlot.addTask<ComplexTask>(followPath);
};

std::vector<vertex_tuple> loadPath(char* str)
{
char pattern[] = "[a-z|A-Z] .*?[a-z|A-Z]"; // шаблон (регул€рное выражение)
   char point[] = "[-\\d]*\\.*\\d*,[-\\d]*\\.*\\d*"; // шаблон (регул€рное выражение)
//   char str[] = "m 148.57143,503.79075 l 5.71428,348.57143 85.71429,0 -17.14286,-354.28571 -74.28571,5.71428 z";  // разбираема€ строка
   //char str[] = "eseses";  // разбираема€ строка
    std::vector<std::string> commands;
    std::vector<std::vector<vertex_tuple> > sequences;

    vertex_tuple curPos;


   // компилирование регул€рного выражени€ во внутреннее представление
   pcre *re;
   pcre *ptpattern;
   int options = 0;
   const char *error;
   int erroffset;
   re = pcre_compile ((char *) pattern, options, &error, &erroffset, NULL);
   ptpattern = pcre_compile ((char *) point, options, &error, &erroffset, NULL);


   if (!re){ // в случае ошибки компил€ции
        cout << "Failed\n";
   }
   else{
      int count = 1;
      int ovector[30];

      const char* buff2;
      ovector[1]=1;

       int i=0;
       while (count>0){
            count = pcre_exec (re, NULL, (char *) str, strlen(str), ovector[1]-1, 0, ovector, 30);

            for (int c = 0; c < 2 * count; c += 2){
            if (ovector[c] < 0){ // или <unset> дл€ несопоставившихс€ подвыражений
               cout << "<unset>\n";
            }
            else{
               pcre_get_substring(str,ovector,count,0,&buff2);
               commands.push_back(buff2);
               cout << commands[i] << "\n";
               i++;
            }
         }
      }
   }

   if (!ptpattern){ // в случае ошибки компил€ции
      cout << "Failed\n";
   }
   else{

      for (int i=0; i<commands.size();i++)
      {

        int count = 1;
        int ovector[30];

        ovector[1]=1;

        const char* temp=commands[i].c_str();

         std::vector < vertex_tuple > poly_temp;
         sequences.push_back(poly_temp);


          while (count >0)
          {

            count = pcre_exec (ptpattern, NULL, (char *)temp ,strlen(temp) , ovector[1]+1, 0, ovector, 30);
             //вывод пар {начало, конец} совпадени€
             for (int c = 0; c < 2 * count; c += 2)
             {
                const char* tempcoords;
                float pointx,pointy;
                char separator;

                pcre_get_substring(temp,ovector,count,0,&tempcoords);
                std::string coords;
                coords = std::string(tempcoords);
                std::istringstream coordstr(coords);
                coordstr>>pointx>>separator>>pointy;

                if ((temp[0]=='l')||((temp[0]=='m')&&(sequences[i].size()>0)))
                {
                    pointx=pointx+curPos[0];
                    pointy=pointy+curPos[1];
                }
                vertex_tuple nextVertex;
                nextVertex.push_back(pointx);
                nextVertex.push_back(pointy);

                sequences[i].push_back(nextVertex);

                curPos.clear();

                curPos.push_back(pointx);
                curPos.push_back(pointy);

                cout<<sequences[i].back()[0]<<","<<sequences[i].back()[1]<<"\n";
                //cout<<pointx<<separator<<pointy<<"\n";
             }
           }
        }
     }

    //vertex_tuple nextVertex;
   /* nextVertex.reserve(10);
    nextVertex.resize(10);
    nextVertex.push_back(0);
    nextVertex.push_back(0);
    */
    std::vector < vertex_tuple > path_curve;


   //for (vector<vector<vertex_tuple> >::iterator i1 = sequences.begin();i1 != sequences.end();++i1)
   // for (vector<vertex_tuple>::iterator i2 = i1->begin();i2!=i1->end();++i2)
   for (int i=0;i<sequences.size();i++)
    for (int j=0;j<sequences[i].size();j++)
    {
    vertex_tuple nextVertex;
        nextVertex.push_back(sequences[i][j][0]);
        nextVertex.push_back(sequences[i][j][1]);
        path_curve.push_back(nextVertex);
    }
path_curve.pop_back();

   return path_curve;
};

VisiLibity::Point normal(VisiLibity::Point Vector)
{
        VisiLibity::Point result=*(new VisiLibity::Point(-1.0*Vector.y(),Vector.x()));
        return 1/sqrt(result.x()*result.x() + result.y()*result.y())*result;
}

Polygon_2 ngon(int n, float r)
{
    Polygon_2 p;

    for (int i=0;i<n;i++)
    {       float PI=std::atan(1.0f)*4.0f;
            p.push_back(Point_2(r*cos(2*PI/n*i)+guest1.pos.x(),r*sin(2*PI/n*i)+guest1.pos.y()));
    }
    return p;
}

VisiLibity::Polygon ConvertPolygonCGAL2Vis(Polygon_2 pgn)
{
    VisiLibity::Polygon cPoly;
    Polygon_2::Vertex_iterator cv ;
    for (cv=pgn.vertices_begin();cv!=pgn.vertices_end();++cv)
          {
              cPoly.push_back(VisiLibity::Point(cv->x(),cv->y()));
          };
    return cPoly;

}


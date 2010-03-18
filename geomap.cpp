#include "geomap.hpp"
#include "globals.hpp"
#include "pcre.h"
geoData sceneVertices;
VisiLibity::Visibility_Polygon visiPoly;
GLUtesselator *tess;


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


geoData mapload(char* path)
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

    std::vector < VisiLibity::Point > poly_temp;

    //geoData vertices_temp(10,poly_temp);
    geoData vertices_temp;

    int i=0;

    while (n1)
    {

        vertices_temp.push_back(loadPath(n1->first_attribute("d")->value()));
        n1=n1->next_sibling("path");
        i++;
    }

    return vertices_temp;

};

void mapdraw()
{
        char cd[_MAX_PATH];
    getcwd(cd, _MAX_PATH);
    //cout << cd;
    strcat(cd,"\\map.svg");

    sceneVertices = mapload(cd);
    std::vector<VisiLibity::Polygon> envPolys;

    for (int i=0;i<sceneVertices.size();i++){
        envPolys.push_back(VisiLibity::Polygon(sceneVertices[i]));
        envPolys[i].eliminate_redundant_vertices(0.0001);
        //VG_Polygon* vgpoly1=Visi2VG(VisiLibity::Polygon(sceneVertices[i]),vgroot);
        //VG_PolygonSetOutline(vgpoly1,1);
    };

    VisiLibity::Environment mapEnv(envPolys);
    //std::cout << mapEnv.h();
    //int holes=mapEnv.h();
    mapEnv.enforce_standard_form();
    VisiLibity::Point pt1;
    //pt1=VisiLibity::Point(160.0f,180.0f);
    pt1=guest1.pos;
    pt1.snap_to_boundary_of(mapEnv);
    pt1.snap_to_vertices_of(mapEnv);

    visiPoly=VisiLibity::Visibility_Polygon(pt1,mapEnv,0.00001);


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

    mapdraw();



	glLoadIdentity();
	glPushAttrib(GL_POLYGON_BIT|GL_LIGHTING_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f,0.0f,0.0f,1.0f);



//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//
//	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
//
//	glShadeModel(shading == FLATSHADING ? GL_FLAT : GL_SMOOTH);

//	glEnable(GL_LIGHT1);
//	glEnable(GL_DEPTH_TEST);
//	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_POLYGON);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
//	glShadeModel(shading == FLATSHADING ? GL_FLAT : GL_SMOOTH);
//
//	pos[0] = 10.0f;
//	pos[1] = 10.0f;
//	pos[2] = 0.0f;
//	pos[3] = 1.0f;
//	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 10.0f);
//	glLightfv(GL_LIGHT1, GL_POSITION, pos);
//
//	pos[1] = -10.0f;
//	pos[2] = 10.0f;
//	glLightfv(GL_LIGHT0, GL_POSITION, pos);
//	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 10.0f);
//
//	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
//	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
//


	glPushMatrix();
	glTranslated(0.0, 0.0, vz);
    GLfloat visi[4]={0.0,0.0,0.0,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, visi);
    glColor3f(0.9,0.9,0.6);
    for (int i=0;i<sceneVertices.size();i++){
        glBegin(GL_LINE_LOOP);
        for (int j=0;j<sceneVertices[i].size();j++){
            glVertex3f( sceneVertices[i][j].x(), sceneVertices[i][j].y(), 0.0f);
        };
        //glVertex3f( sceneVertices[i][0].x(), sceneVertices[i][0].y(), 0.0f);
        glEnd();
    };


    gluTessBeginPolygon (tess, NULL);
    gluTessBeginContour (tess);
    GLdouble data[visiPoly.n()][3];

    for (i=0; i<visiPoly.n();i++)
    {
        data[i][0]=visiPoly[i].x();
        data[i][1]=visiPoly[i].y();
        data[i][2]=0;
        gluTessVertex (tess, data[i], data[i]);
    }
    gluTessEndContour (tess);
    gluEndPolygon (tess);

    glBegin(GL_LINE_LOOP);
        for (int i=0;i<visiPoly.n();i++){
            glVertex3f(visiPoly[i].x(),visiPoly[i].y(),0.0f);
        }
    glEnd();

    //glPopMatrix();
    GLfloat marker[4]={1.0,0.0,0.0,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, marker);
    glColor3f(1.0,0.0,0.0);
    guest1.Draw();

    glPopMatrix();
    glPopAttrib();

    };
void
MapScaleFunction(AG_Event *event)
{
	GLdouble xMin, xMax, yMin, yMax;

	glLoadIdentity();

	/* Set a 60 degrees field of view with 1.0 aspect ratio. */
	/*yMax = 0.01*tan(0.523598f);
	yMin = -yMax;
	xMin = yMin;
	xMax = yMax;
	*/
	yMin=0;
	yMax=1000;
	xMin=0;
	xMax=1024;


	glOrtho(xMin, xMax, yMin, yMax, 0.1, 100.0);
};

std::vector<VisiLibity::Point> loadPath(char* str)
{
char pattern[] = "[a-z|A-Z] .*?[a-z|A-Z]"; // шаблон (регулярное выражение)
   char point[] = "[-\\d]*\\.*\\d*,[-\\d]*\\.*\\d*"; // шаблон (регулярное выражение)
//   char str[] = "m 148.57143,503.79075 l 5.71428,348.57143 85.71429,0 -17.14286,-354.28571 -74.28571,5.71428 z";  // разбираемая строка
   //char str[] = "eseses";  // разбираемая строка
    std::vector<std::string> commands;
    std::vector<std::vector<VisiLibity::Point> > sequences;

    VisiLibity::Point curPos;


   // компилирование регулярного выражения во внутреннее представление
   pcre *re;
   pcre *ptpattern;
   int options = 0;
   const char *error;
   int erroffset;
   re = pcre_compile ((char *) pattern, options, &error, &erroffset, NULL);
   ptpattern = pcre_compile ((char *) point, options, &error, &erroffset, NULL);


   if (!re){ // в случае ошибки компиляции
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
            if (ovector[c] < 0){ // или <unset> для несопоставившихся подвыражений
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

   if (!ptpattern){ // в случае ошибки компиляции
      cout << "Failed\n";
   }
   else{

      for (int i=0; i<commands.size();i++)
      {

        int count = 1;
        int ovector[30];

        ovector[1]=1;

        const char* temp=commands[i].c_str();

         std::vector < VisiLibity::Point > poly_temp;
         sequences.push_back(poly_temp);


          while (count >0)
          {

            count = pcre_exec (ptpattern, NULL, (char *)temp ,strlen(temp) , ovector[1]+1, 0, ovector, 30);
             //вывод пар {начало, конец} совпадения
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
                    pointx=pointx+curPos.x();
                    pointy=pointy+curPos.y();
                }
                sequences[i].push_back(VisiLibity::Point(pointx,pointy));
                curPos=VisiLibity::Point(pointx,pointy);
                cout<<sequences[i].back().x()<<","<<sequences[i].back().y()<<"\n";
                //cout<<pointx<<separator<<pointy<<"\n";
             }
           }
        }
     }
   std::vector < VisiLibity::Point > path;

   for (vector<vector<VisiLibity::Point> >::iterator i1 = sequences.begin();i1 != sequences.end();i1++)
    for (vector<VisiLibity::Point>::iterator i2 = i1->begin();i2!=i1->end();i2++)
        path.push_back(*i2);

   return path;
};


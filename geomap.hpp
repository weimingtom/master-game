#ifndef _GEOMAP_H_
#define _GEOMAP_H_

#include <agar/core.h>
#include <agar/gui.h>

#include "visilibity/visilibity.hpp"

#include <vector>
#include <sstream>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

//#include <direct.h>

#include <agar/gui/opengl.h>

#include <GL/glu.h>


#include <CGAL/Cartesian.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <list>

#include <agar/core/types.h>


// instead of
typedef CGAL::Cartesian<double>               Kernel;
// workaround for VC++
//struct Kernel : public CGAL::Cartesian<Number_type> {};

typedef Kernel::Point_2                            Point_2;
typedef CGAL::Polygon_2<Kernel>                    Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>         Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>            Pwh_list_2;
typedef CGAL::Polygon_set_2<Kernel>                Polygon_set_2;
typedef std::vector<double> vertex_tuple;



typedef std::vector < std::vector < vertex_tuple> > geoData;

void mapdraw();
VisiLibity::Polygon ConvertPolygonCGAL2Vis(Polygon_2 pgn);
VisiLibity::Point normal(VisiLibity::Point Vector);
geoData mapload(char* path,VisiLibity::Environment & mapEnv,VisiLibity::Visibility_Graph & visGraph,float clearDist);
std::vector<vertex_tuple> loadPath(char* str);
void MapDrawFunction(AG_Event *event);
void MapScaleFunction(AG_Event *event);
void MapClickFunction(AG_Event *event);
void MapMoveFunction(AG_Event *event);

Polygon_2 ngon(int n, float r);
#endif

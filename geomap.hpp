#ifndef _GEOMAP_H_
#define _GEOMAP_H_

#include <core.h>
#include <gui.h>
#include <agar/vg.h>

#include "visilibity.hpp"

#include <vector>
#include <sstream>

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include <direct.h>

#include <agar/gui/opengl.h>

#include <GL/glu.h>


typedef std::vector < std::vector < VisiLibity::Point > > geoData;

void mapdraw();
VisiLibity::Point normal(VisiLibity::Point Vector);
geoData mapload(char* path,VisiLibity::Environment & mapEnv,VisiLibity::Visibility_Graph & visGraph,float clearDist);
std::vector<VisiLibity::Point> loadPath(char* str);
void MapDrawFunction(AG_Event *event);
void MapScaleFunction(AG_Event *event);

#endif

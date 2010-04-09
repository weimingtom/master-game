#ifndef __GLOBALS_INCLUDED__
#define __GLOBALS_INCLUDED__
#include "entity.hpp"
#include "events.hpp"
#include "geomap.hpp"


extern Entity guest1;
extern Slot<int> UpdateTimerSlot;

extern int mapYMin;
extern int mapYMax;
extern int mapXMin;
extern int mapXMax;

extern geoData sceneVertices;
extern geoData sceneVerticesCollision;

extern VisiLibity::Environment mapEnv;
extern VisiLibity::Visibility_Graph visGraph;

extern VisiLibity::Environment mapEnvCollision;
extern VisiLibity::Visibility_Graph visGraphCollision;


extern VisiLibity::Polyline motionPath;


#endif


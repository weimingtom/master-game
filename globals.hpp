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

extern float mapCenterX;
extern float mapCenterY;
extern float mapWidth;
extern float mapHeight;

extern geoData sceneVertices;
extern geoData sceneVerticesCollision;

extern VisiLibity::Environment mapEnv;
extern VisiLibity::Visibility_Graph visGraph;

extern VisiLibity::Environment mapEnvCollision;
extern VisiLibity::Visibility_Graph visGraphCollision;


extern VisiLibity::Polyline motionPath;

extern AG_Surface *surf1;

extern GLint viewport[4];
extern GLdouble modelview[16];
extern GLdouble projection[16];


//extern float cursorX,cursorY;
extern float cursorwX,cursorwY;

//extern AG_Timeout *TO;

//extern ComplexTask followPath;

//extern goToPoint order1;

template <typename PARAM>
Uint32 timerFunc(void *obj, Uint32 ival, void *arg)
{
    Slot<PARAM> b1=*(Slot<PARAM>*)arg;
    b1(ival);
    //UpdateTimerSlot(ival);
    return 1;
};


#endif


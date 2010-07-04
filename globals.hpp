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

extern AG_Surface *surf1;

<<<<<<< local
extern ComplexTask followPath;
=======
extern float cursorX,cursorY;

//extern AG_Timeout *TO;

//extern ComplexTask followPath;

//extern goToPoint order1;

template <typename PARAM>
Uint32 timerFunc(void *obj, Uint32 ival, void *arg)
{
    static Slot<PARAM> b1=**(Slot<PARAM>**)arg;
    //b1(ival);
    UpdateTimerSlot(ival);
    return 1;
};

>>>>>>> other

#endif


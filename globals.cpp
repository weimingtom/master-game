#include "globals.hpp"
//Глобальные переменные
Entity guest1=  *(new Entity(160,180));

Slot<int> UpdateTimerSlot;

int mapYMin=0;
int mapYMax=650;
int mapXMin=0;
int mapXMax=650;

geoData sceneVertices;
geoData sceneVerticesCollision;

VisiLibity::Environment mapEnv;
VisiLibity::Visibility_Graph visGraph;

VisiLibity::Environment mapEnvCollision;
VisiLibity::Visibility_Graph visGraphCollision;

VisiLibity::Polyline motionPath;

AG_Surface *surf1;

float cursorX,cursorY;

//AG_Timeout *TO;

//ComplexTask followPath;
//goToPoint order1;

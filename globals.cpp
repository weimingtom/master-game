#include "globals.hpp"
//Глобальные переменные
Entity guest1=  *(new Entity(160,180));

Slot<int> UpdateTimerSlot;

int mapYMin=0;
int mapYMax=1000;
int mapXMin=0;
int mapXMax=1024;

geoData sceneVertices;
geoData sceneVerticesCollision;

VisiLibity::Environment mapEnv;
VisiLibity::Visibility_Graph visGraph;

VisiLibity::Environment mapEnvCollision;
VisiLibity::Visibility_Graph visGraphCollision;


VisiLibity::Polyline motionPath;

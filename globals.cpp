#include "globals.hpp"
//Глобальные переменные
Entity guest1=  *(new Entity(160,180));

Slot<int> UpdateTimerSlot;
Slot<int> UpdateTimerOnceSlot;

int mapYMin=0;
int mapYMax=1200;
int mapXMin=0;
int mapXMax=1600;

float mapCenterX=400;
float mapCenterY=300;
float mapWidth=1024;
float mapHeight=768;



geoData sceneVertices;
geoData sceneVerticesCollision;

VisiLibity::Environment mapEnv;
VisiLibity::Visibility_Graph visGraph;

VisiLibity::Environment mapEnvCollision;
VisiLibity::Visibility_Graph visGraphCollision;

VisiLibity::Polyline motionPath;

AG_Surface *surf1;

GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];


//float cursorX,cursorY;
float cursorwX,cursorwY;
//AG_Timeout *TO;
//ComplexTask followPath;
//goToPoint order1;

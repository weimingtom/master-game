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
float cursorwX,cursorwY;


ComplexTask planPath(float cursorwX, float cursorwY)
{
    VisiLibity::Point cp1 = VisiLibity::Point(cursorwX,cursorwY);
    if (cp1.in(mapEnvCollision))
    {
        motionPath =mapEnvCollision.shortest_path(guest1.pos,cp1,visGraphCollision,5);
        static goToPoint orderChain[10];
        orderChain[0]=*(new goToPoint(motionPath[1].x(),motionPath[1].y(),&guest1));
        static ComplexTask followPath;
        followPath=ComplexTask(orderChain[0]);
        for (int i =2;i<motionPath.size();i++){
            orderChain[i-1]=*(new goToPoint(motionPath[i].x(),motionPath[i].y(),&guest1));
            followPath.AddAction(orderChain[i-1]);
        }
        return followPath;
};
}

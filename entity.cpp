#include "entity.hpp"
#include "GL/gl.h"


Entity::Entity()
{
    pos.first=0;
    pos.second=0;
};

Entity::Entity(int initX,int initY)
{
    pos.first=initX;
    pos.second=initY;
};

void Entity::Update(int time)
{
};
void Entity::Draw()
{

glColor3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
        glVertex2f(pos.first-0.5,pos.second-0.5);
        glVertex2f(pos.first-0.5,pos.second+0.5);
        glVertex2f(pos.first+0.5,pos.second+0.5);
        glVertex2f(pos.first+0.5,pos.second-0.5);
    glEnd();
};

void Entity::setPos(int X, int Y)
{
    pos.first=X;
    pos.second=Y;
};

/*
uint8 line_listener(int x, int y) {
    static float lightEnergy=1;
	if (!TCOD_map_is_walkable(levelMap,x,y)|| !( x>= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT) )
	{
            lightEnergy=1;
            return false;
        }

    lightMap[x][y]=min(1.0f,lightMap[x][y]+lightEnergy);
    lightEnergy=lightEnergy*0.85;
	return true;
};
*/

Light::Light(int initX,int initY)
{
    pos.first=initX;
    pos.second=initY;
};


void Light::updateLightmap()

{
//    memset(lightMap,0,sizeof(float)*MAP_WIDTH*MAP_HEIGHT);
/*
    for (int x=0; x <MAP_WIDTH ; x++) {
        TCOD_line(pos[0],pos[1],x,0,line_listener);
    }

    for (int y=0; y < MAP_HEIGHT; y++ ) {
        TCOD_line(pos[0],pos[1],MAP_WIDTH,y,line_listener);
    }


    for (int x=MAP_WIDTH; x >0 ; x--) {
        TCOD_line(pos[0],pos[1],x,MAP_HEIGHT,line_listener);
    }

   for (int y=MAP_HEIGHT; y >0 ; y-- ) {
        TCOD_line(pos[0],pos[1],0,y,line_listener);
    }

*/
/*
    for (float t=0.0;t<2*3.1451;t=t+0.05)
{
    float radius=100;
    int x=pos[0]+round(radius*cos(t));
    int y=pos[1]+round(radius*sin(t));
    TCOD_line(pos[0],pos[1],x,y,line_listener);
    /*
    TCOD_line(pos[0]+1,pos[1],x,y,line_listener);
    TCOD_line(pos[0]-1,pos[1],x,y,line_listener);
    TCOD_line(pos[0],pos[1]-1,x,y,line_listener);
    TCOD_line(pos[0],pos[1]+1,x,y,line_listener);

}
*/
/*
    for (int y=0; y < MAP_HEIGHT; y++ ) {
        for (int x=0; x < MAP_WIDTH; x++ ) {
            printf("%f ",lightMap[x][y]);
        }
        printf("/n");
    }
*/
};

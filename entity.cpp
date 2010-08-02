#include "entity.hpp"
#include <agar/gui/opengl.h>
Entity::Entity(float initX,float initY)
{
    pos.set_x(initX);
    pos.set_y(initY);
};

void Entity::Update(int time)
{
};
void Entity::Draw()
{
    /*
    glPointSize(10.0);

    glBegin(GL_POINTS);
        glVertex3f(pos.x(),pos.y(),1.0f);
    glEnd();
    */

    glBegin(GL_TRIANGLES);
        glNormal3f(0,0,1);
        glVertex3f(pos.x()-10,pos.y()-10,1.0f);
        glVertex3f(pos.x()+10,pos.y()-10,1.0f);
        glVertex3f(pos.x()-10,pos.y()+10,1.0f);
    glEnd();

};



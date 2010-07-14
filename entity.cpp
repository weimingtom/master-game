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
    glBegin(GL_TRIANGLES);
        glVertex3f(pos.x()-10,pos.y()-10,1.0f);
        glVertex3f(pos.x()+10,pos.y()-10,1.0f);
        glVertex3f(pos.x()-10,pos.y()+10,1.0f);
    glEnd();
};



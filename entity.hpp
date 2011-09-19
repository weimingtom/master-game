#ifndef __ENTITY_INCLUDED__
#define __ENTITY_INCLUDED__

#include <vector>
#include <set>
#include <map>
#include "math.h"
#include "globals.hpp"
#include <stdlib.h>

#include "textUtils.hpp"

#include <Rocket/Core/Texture.h>



struct Sprite
{
    int left;
    int top;
    int bottom;
    int right;

    int textureHandle;
    Rocket::Core::Vector2i texture_dimensions;
};

class Entity
{
    public:

        int objectId;

        vertex_tuple pos;

        Entity();
        Entity(int initX, int initY);

        virtual rapidxml::xml_node<>* Serialize(xmlFile& doc);
        virtual void Deserialize(rapidxml::xml_node<>* node);

        virtual void Draw();
        virtual void Update(int time);


};

class Light: public Entity
{
    public:
        Light(int initX, int initY);
        void updateLightmap();
};

class Guest: public Entity
{
    public:
        int textureHandle;
        int sprite;

        Guest();
        void Draw();
        rapidxml::xml_node<>* Serialize(xmlFile& doc);
        void Deserialize(rapidxml::xml_node<>* node);

};


class PowerSource: public Entity
{
    public:

        void Draw();

};

class PowerConsumer: public Entity
{
    public:

        void Draw();
        rapidxml::xml_node<>* Serialize(xmlFile& doc);
        void Deserialize(rapidxml::xml_node<>* node);



};

class powerGridNode: public Entity
{
    public:
        std::vector < short > children;
};

extern std::map<int,Entity*> gameObjectsTable;
extern std::map<int,Sprite*> spriteTable;


int getUniqueId();
std::vector<Entity*> getObjectsWithCoords(int x, int y);


#endif


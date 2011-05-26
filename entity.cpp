#include "entity.hpp"
#include "GL/gl.h"

std::map<int,Entity*> gameObjectsTable;
std::map<int,Sprite*> spriteTable;

int getUniqueId()
{
    static std::set<int> idTable;
    int id1=rand();
    while (idTable.find(id1)!=idTable.end())
        id1=rand();

    idTable.insert(id1);
    return id1;
};

std::vector<Entity*> getObjectsWithCoords(int x, int y)
{
    std::vector<Entity*> result;
    for (std::map<int,Entity*>::iterator go1=gameObjectsTable.begin();go1!=gameObjectsTable.end();go1++)
    {
        if ((go1->second->pos.first==x) && (go1->second->pos.second==y))
        {
            result.push_back(go1->second);
        }
    };
    return result;

}


Entity::Entity()
{
    pos.first=0;
    pos.second=0;
};

rapidxml::xml_node<>* Entity::Serialize(xmlFile& doc)
{



    //запишем новый


    std::stringstream idStr;
    std::string id;


    rapidxml::xml_node<> *entityNode = doc.allocate_node(rapidxml::node_element,"entity");

    static char buffer [33];
    itoa(objectId,buffer,10);

    rapidxml::xml_attribute<> *idAttr = doc.allocate_attribute("id",buffer);

    entityNode->append_attribute(idAttr);


    static char buffer1 [33];
    static char buffer2 [33];

    itoa(pos.first,buffer1,10);
    itoa(pos.second,buffer2,10);

    strcat(buffer1,",");
    strcat(buffer1,buffer2);

    rapidxml::xml_attribute<> *posAttr = doc.allocate_attribute("pos", buffer1);

    entityNode->append_attribute(posAttr);


    printf("serializing id: \n");
    printf(entityNode->first_attribute("id")->value());


    return entityNode;

};

void Entity::Deserialize(rapidxml::xml_node<>* node)
{
    if (node->first_attribute("id"))
    {
        std::string c1 = node->first_attribute("id")->value();
        objectId = atoi(c1.c_str());
    };

    if (node->first_attribute("pos"))
    {
        std::string c2 = node->first_attribute("pos")->value();
        std::vector<std::string> pairs = split(c2,',');
        pos.first=atoi(pairs[0].c_str());
        pos.second=atoi(pairs[1].c_str());
    };


};

Entity::Entity(int initX,int initY)
{
    pos.first=initX;
    pos.second=initY;

    //Rocket::Core::Texture texture;
    //texture.Load(".\\assets\\entity.tga");

    //texture.GetHandle()
};

void Entity::Update(int time)
{
};
void Entity::Draw()
{
};
/*
void Entity::setPos(int X, int Y)
{
    pos.first=X;
    pos.second=Y;
};
*/

Light::Light(int initX,int initY)
{
    pos.first=initX;
    pos.second=initY;
};


void Light::updateLightmap()
{

};

void PowerSource::Draw()
{
    glColor3f(0.5,0.0,0.0);
	glBegin(GL_QUADS);
        glVertex2f(pos.first-0.5,pos.second-0.5);
        glVertex2f(pos.first-0.5,pos.second+0.5);
        glVertex2f(pos.first+0.5,pos.second+0.5);
        glVertex2f(pos.first+0.5,pos.second-0.5);
    glEnd();

};

void PowerConsumer::Draw()
{
    glColor3f(0.0,0.5,0.0);
	glBegin(GL_QUADS);
        glVertex2f(pos.first-0.5,pos.second-0.5);
        glVertex2f(pos.first-0.5,pos.second+0.5);
        glVertex2f(pos.first+0.5,pos.second+0.5);
        glVertex2f(pos.first+0.5,pos.second-0.5);
    glEnd();

};


rapidxml::xml_node<>* PowerConsumer::Serialize(xmlFile& doc)
{
    rapidxml::xml_node<>* v1 =  Entity::Serialize(doc);

    char *node_name = doc.allocate_string("powerConsumer");        // Allocate string and copy name into it
    rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, node_name);  // Set node name to node_name
    node = doc.clone_node(v1);

    return node;
};


void PowerConsumer::Deserialize(rapidxml::xml_node<>* node)
{

    Entity::Deserialize(node);
};



Guest::Guest()
{
    pos.first=0;
    pos.second=0;
};


rapidxml::xml_node<>* Guest::Serialize(xmlFile& doc)
{
    rapidxml::xml_node<>* v1 =  Entity::Serialize(doc);

    char *node_name = doc.allocate_string("guest");        // Allocate string and copy name into it

    v1->name(node_name);

    rapidxml::xml_attribute<>* sp1 = doc.allocate_attribute("sprite");

    char buffer1 [33];

    itoa(sprite,buffer1,10);

    char *spriteNum = doc.allocate_string(buffer1);

    sp1->value(spriteNum);

    v1->append_attribute(sp1);

    return v1;
};


void Guest::Deserialize(rapidxml::xml_node<>* node)
{
    Entity::Deserialize(node);

    if (node->first_attribute("sprite"))
    {
        sprite =  atoi(node->first_attribute("sprite")->value());
        printf("guest's sprite: %i \n",sprite);
    };

};

void Guest::Draw()
{

    float vertices[8]={-1,-1,-1,1,1,1,1,-1};
    //float texcoords[8]={0.2,0.6,0.2,0.2,0.6,0.2,0.6,0.6};
    float texcoords[8]={0.0,1.0,0.0,0.0,1.0,0.0,1.0,1.0};
    unsigned char colors[32] = {255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255};


    Sprite curSprite=*spriteTable[sprite];

    texcoords[0]=(float) curSprite.left/curSprite.texture_dimensions.x;
    texcoords[1]=(float) curSprite.bottom/curSprite.texture_dimensions.y;

    texcoords[2]=(float)curSprite.left/curSprite.texture_dimensions.x;
    texcoords[3]=(float) curSprite.top/curSprite.texture_dimensions.y;

    texcoords[4]=(float) curSprite.right/curSprite.texture_dimensions.x;
    texcoords[5]=(float) curSprite.top/curSprite.texture_dimensions.y;

    texcoords[6]=(float) curSprite.right/curSprite.texture_dimensions.x;
    texcoords[7]=(float) curSprite.bottom/curSprite.texture_dimensions.y;
    //printf("%d",texcoords[7]);


    glPushMatrix();

    glTranslatef(pos.first,pos.second,0.0);

    glVertexPointer(2, GL_FLOAT,0, vertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, (GLuint) spriteTable[sprite]->textureHandle);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT,0, texcoords);

    unsigned int indices[6]={0,1,2,2,3,0};

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
};


#include "CompVisualNetwork.hpp"
#include "CompVisualNetworkTemplate.hpp"
#include "Object.hpp"

/*static*/ comp_id_type CompVisualNetwork::mFamilyID = "CompVisual";
/*static*/ comp_id_type CompVisualNetwork::mComponentID = "CompVisualNetwork";

CompVisualNetwork::CompVisualNetwork(CompVisualNetworkTemplate *templ) {
	// Construct

}


rapidxml::xml_node<>* CompVisualNetwork::Serialize(xmlFile& doc)
{

    rapidxml::xml_node<> *entityNode = doc.allocate_node(rapidxml::node_element,"CompVisualNetwork");

    return entityNode;
};


void CompVisualNetwork::Deserialize(rapidxml::xml_node<>* node)
{

};


void CompVisualNetwork::Draw()
{
    /*
    float vertices[8]={-0.5,-0.5,-0.5,0.5,0.5,0.5,0.5,-0.5};
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

    vertex_tuple pos = static_cast<CompPhys*>(getOwnerObject()->getComponent("CompNetwork"))->pos;

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
    */
};

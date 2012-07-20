
#include "CompVisualNetwork.hpp"
#include "CompVisualNetworkTemplate.hpp"
#include "Object.hpp"
#include "CompNetwork.hpp"

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

    glPushMatrix();
    CompNetwork* net1 = static_cast<CompPhys*>(getOwnerObject()->getComponent("CompNetwork");
    vertex_tuple pos = static_cast<CompPhys*>()->pos;

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

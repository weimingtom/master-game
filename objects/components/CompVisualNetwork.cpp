
#include "CompVisualNetwork.hpp"
#include "CompVisualNetworkTemplate.hpp"
#include "Object.hpp"
#include "CompNetwork.hpp"
#include "CompNetworkNode.hpp"
#include "CompPhys.hpp"
#include <ShellOpenGL.h>
#include <iostream>


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
    Component::Deserialize(node);

};


void CompVisualNetwork::Draw()
{


    //glPushMatrix();

    Object* networkObject = gameObjectsTable["network"];

    CompNetwork* net1 = static_cast<CompNetwork*>(networkObject->getComponent("CompNetwork"));
    for (CompNetwork::net_iterator it = net1->network_map.begin(); it!= net1->network_map.end(); it++)
    {
        CompNetwork::net_element_iterator eit = net1->network_map.equal_range(it->first);
        for (CompNetwork::net_iterator neigh = eit.first; neigh!= eit.second; neigh++)
        {
            //std::cout << neigh->first << ","<<neigh->second;

            CompNetworkNode* nn1 = static_cast<CompNetworkNode*>(gameComponentsTable[neigh->first]);
            CompNetworkNode* nn2 = static_cast<CompNetworkNode*>(gameComponentsTable[neigh->second]);

            //Object* o1 = nn1->getOwnerObject();
            //printf(nn1->componentID().c_str());

            vertex_tuple pos1 = static_cast<CompPhys*>(nn1->getOwnerObject()->getComponent("CompPhys"))->pos;
            vertex_tuple pos2 = static_cast<CompPhys*>(nn2->getOwnerObject()->getComponent("CompPhys"))->pos;

            //std::cout << pos1.first << ","<<pos1.second;
            glColor4f(1.0,1.0,1.0,1.0);
            glBegin(GL_LINES);
                glVertex2f(pos1.first,pos1.second);
                glVertex2f(pos2.first,pos2.second);
            glEnd();

            glBegin(GL_POINTS);
                glVertex2f(pos1.first,pos1.second);
                glVertex2f(pos2.first,pos2.second);
            glEnd();



        };
    };

/*
    //CompNetwork* net1 = static_cast<CompNetwork*>(getOwnerObject()->getComponent("CompNetwork");
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

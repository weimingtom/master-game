
#include "CompNetworkNode.hpp"
#include "CompNetworkNodeTemplate.hpp"

//#include <cstring>
#include <vector>
#include "../../textUtils.hpp"



/*static*/ comp_id_type CompNetworkNode::mFamilyID = "CompNetwork";
/*static*/ comp_id_type CompNetworkNode::mComponentID = "CompNetworkNode";


CompNetworkNode::CompNetworkNode(CompNetworkNodeTemplate *templ) {
	// Construct
    id=templ->id;
}


rapidxml::xml_node<>* CompNetworkNode::Serialize(xmlFile& doc)
{

    //запишем новый

    rapidxml::xml_node<> *entityNode = doc.allocate_node(rapidxml::node_element,"CompNetworkNode");

    rapidxml::xml_attribute<> *idAttr = doc.allocate_attribute("id", id.c_str());

    entityNode->append_attribute(idAttr);



    return entityNode;
};


void CompNetworkNode::Deserialize(rapidxml::xml_node<>* node)
{
    if (node->first_node("id"))
    {
        id=node->first_node("id")->first_attribute("value")->value();
    }
};



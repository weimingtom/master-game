
#include "CompPhys.hpp"
#include "CompPhysTemplate.hpp"

//#include <cstring>
#include <vector>
#include "../../textUtils.hpp"
#include <sstream>



/*static*/ comp_id_type CompPhys::mFamilyID = "CompPhys";
/*static*/ comp_id_type CompPhys::mComponentID = "CompPhys";


CompPhys::CompPhys(CompPhysTemplate *templ) {
	// Construct
	pos=templ->pos;
	passable=templ->passable;

}


rapidxml::xml_node<>* CompPhys::Serialize(xmlFile& doc)
{

    //запишем новый

    std::string id;

    rapidxml::xml_node<> *entityNode = doc.allocate_node(rapidxml::node_element,"goc");

    static char buffer0 [33];

    strcpy(buffer0,"CompPhys");

    rapidxml::xml_attribute<> *nameAttr = doc.allocate_attribute("name", buffer0);

    entityNode->append_attribute(nameAttr);

    rapidxml::xml_node<> *posNode = doc.allocate_node(rapidxml::node_element,"pos");

    static char buffer1 [33];
    static char buffer2 [33];

    itoa(pos.first,buffer1,10);
    itoa(pos.second,buffer2,10);

    strcat(buffer1,",");
    strcat(buffer1,buffer2);

    rapidxml::xml_attribute<> *posAttr = doc.allocate_attribute("value", buffer1);

    posNode->append_attribute(posAttr);

    entityNode->append_node(posNode);

    rapidxml::xml_node<> *passableNode = doc.allocate_node(rapidxml::node_element,"passable");

    static char buffer3 [33];

    itoa(passable,buffer3,10);

    rapidxml::xml_attribute<> *passableAttr = doc.allocate_attribute("value", buffer3);

    passableNode->append_attribute(passableAttr);

    entityNode->append_node(passableNode);

    return entityNode;
};


void CompPhys::Deserialize(rapidxml::xml_node<>* node)
{

    if (node->first_node("pos"))
    {
        std::string c2 = node->first_node("pos")->first_attribute("value")->value();
        std::vector<std::string> pairs = split(c2,',');
        pos.first=atoi(pairs[0].c_str());
        pos.second=atoi(pairs[1].c_str());
    };

    if (node->first_node("passable"))
    {
        std::string c2 = node->first_node("passable")->first_attribute("value")->value();
        std::istringstream (c2)>>passable;
    };

};



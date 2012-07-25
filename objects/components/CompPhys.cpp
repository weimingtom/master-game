
#include "CompPhys.hpp"
#include "CompPhysTemplate.hpp"

//#include <cstring>
#include <vector>
#include "../../textUtils.hpp"



/*static*/ comp_id_type CompPhys::mFamilyID = "CompPhys";
/*static*/ comp_id_type CompPhys::mComponentID = "CompPhys";


CompPhys::CompPhys(CompPhysTemplate *templ) {
	// Construct
	pos=templ->pos;

}


rapidxml::xml_node<>* CompPhys::Serialize(xmlFile& doc)
{

    //запишем новый

    std::string id;


    rapidxml::xml_node<> *entityNode = doc.allocate_node(rapidxml::node_element,"CompPhys");

    static char buffer1 [33];
    static char buffer2 [33];

    sprintf(buffer1, "%d", pos.first);
    sprintf(buffer2, "%d", pos.second);
    //itoa(pos.first,buffer1,10);
    //itoa(pos.second,buffer2,10);

    strcat(buffer1,",");
    strcat(buffer1,buffer2);

    rapidxml::xml_attribute<> *posAttr = doc.allocate_attribute("pos", buffer1);

    entityNode->append_attribute(posAttr);


    return entityNode;
};


void CompPhys::Deserialize(rapidxml::xml_node<>* node)
{

    Component::Deserialize(node);

    if (node->first_node("pos"))
    {
        std::string c2 = node->first_node("pos")->first_attribute("value")->value();
        std::vector<std::string> pairs = split(c2,',');
        pos.first=atoi(pairs[0].c_str());
        pos.second=atoi(pairs[1].c_str());
    };


};




#include "CompNetwork.hpp"
#include "CompNetworkTemplate.hpp"

//#include <cstring>
#include <vector>
#include "../../textUtils.hpp"



/*static*/ comp_id_type CompNetwork::mFamilyID = "CompNetwork";
/*static*/ comp_id_type CompNetwork::mComponentID = "CompNetwork";


CompNetwork::CompNetwork(CompNetworkTemplate *templ) {
	// Construct
	pos=templ->pos;

}


rapidxml::xml_node<>* CompNetwork::Serialize(xmlFile& doc)
{

    //запишем новый

    std::string id;


    rapidxml::xml_node<> *entityNode = doc.allocate_node(rapidxml::node_element,"CompNetwork");

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


void CompNetwork::Deserialize(rapidxml::xml_node<>* node)
{

    if (node->first_node("pos"))
    {
        std::string c2 = node->first_node("pos")->first_attribute("value")->value();
        std::vector<std::string> pairs = split(c2,',');
        pos.first=atoi(pairs[0].c_str());
        pos.second=atoi(pairs[1].c_str());
    };


};



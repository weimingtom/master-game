
#include "CompNetwork.hpp"
#include "CompNetworkTemplate.hpp"

//#include <cstring>
#include <vector>
#include "../../textUtils.hpp"



/*static*/ comp_id_type CompNetwork::mFamilyID = "CompNetwork";
/*static*/ comp_id_type CompNetwork::mComponentID = "CompNetwork";


CompNetwork::CompNetwork(CompNetworkTemplate *templ) {
	// Construct
	network_map=multimap<obj_id_type,obj_id_type>(templ->network_map);

}


rapidxml::xml_node<>* CompNetwork::Serialize(xmlFile& doc)
{

    //запишем новый

    rapidxml::xml_node<> *entityNode = doc.allocate_node(rapidxml::node_element,"CompNetwork");
    std::string netstr;

    for (net_iterator i1=network_map.begin();i1!=network_map.end();i1++)
    {
        netstr=netstr+i1->first + "," + i1->second+" ";
        //sprintf(buffer1, "%d", pos.first);
        //sprintf(buffer2, "%d", pos.second);
        //itoa(pos.first,buffer1,10);
        //itoa(pos.second,buffer2,10);
    }

    rapidxml::xml_attribute<> *mapAttr = doc.allocate_attribute("map", netstr.c_str());

    entityNode->append_attribute(mapAttr);


    return entityNode;
};


void CompNetwork::Deserialize(rapidxml::xml_node<>* node)
{
    if (node->first_node("map"))
    {
        std::string c2 = node->first_node("map")->first_attribute("value")->value();
        std::vector<std::string> pairs = split(c2,' ');
        for (std::vector<std::string>::iterator i1 = pairs.begin(); i1!=pairs.end(); i1++)
        {
            std::vector<std::string> edge = split(*i1,',');
            network_map.insert(pair<obj_id_type,obj_id_type>(edge[0],edge[1]));
        }
    };

};



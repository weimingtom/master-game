
#include "CompNetworkTemplate.hpp"
#include "../../textUtils.hpp"
#include <cstring>
#include <vector>



/*static*/ comp_id_type CompNetworkTemplate::mComponentID = "CompNetwork" ;
/*static*/ comp_id_type CompNetworkTemplate::mFamilyID = "CompNetwork" ;


void CompNetworkTemplate::Deserialize(rapidxml::xml_node<>* node)
{

    if (node->first_node("map"))
    {
        std::string c2 = node->first_node("map")->first_attribute("value")->value();
        std::vector<std::string> pairs = split(c2,' ');
        for (std::vector<std::string>::iterator i1 = pairs.begin(); i1!=pairs.end(); i1++)
        {
            std::vector<std::string> edge = split(*i1,',');
            network_map.insert(pair<comp_id_type,comp_id_type>(edge[0],edge[1]));
        }
    };
};


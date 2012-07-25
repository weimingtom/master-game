
#include "CompNetworkNodeTemplate.hpp"
#include "../../textUtils.hpp"
#include <cstring>
#include <vector>



/*static*/ comp_id_type CompNetworkNodeTemplate::mComponentID = "CompNetworkNode" ;
/*static*/ comp_id_type CompNetworkNodeTemplate::mFamilyID = "CompNetwork" ;


void CompNetworkNodeTemplate::Deserialize(rapidxml::xml_node<>* node)
{
    /*
    if (node->first_node("id"))
    {
        id=node->first_node("id")->first_attribute("value")->value();
    }
    */

        if (node->first_node("network"))
    {
        id=node->first_node("network")->first_attribute("value")->value();
    }

};


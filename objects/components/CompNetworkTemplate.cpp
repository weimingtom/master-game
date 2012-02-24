
#include "CompNetworkTemplate.hpp"
#include "../../textUtils.hpp"
#include <cstring>
#include <vector>



/*static*/ comp_id_type CompNetworkTemplate::mComponentID = "CompNetwork" ;
/*static*/ comp_id_type CompNetworkTemplate::mFamilyID = "CompNetwork" ;


void CompNetworkTemplate::Deserialize(rapidxml::xml_node<>* node)
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


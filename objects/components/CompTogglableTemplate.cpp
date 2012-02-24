
#include "CompTogglableTemplate.hpp"
#include "../../textUtils.hpp"
#include <cstring>
#include <vector>



/*static*/ comp_id_type CompTogglableTemplate::mComponentID = "CompTogglable" ;
/*static*/ comp_id_type CompTogglableTemplate::mFamilyID = "CompTogglable" ;


void CompTogglableTemplate::Deserialize(rapidxml::xml_node<>* node)
{

   if (node->first_node("state"))
    {
        std::string c2 = node->first_node("state")->first_attribute("value")->value();
        state=atoi(c2.c_str());
    };


};


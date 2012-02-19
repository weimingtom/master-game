
#include "CompVisualSqTemplate.hpp"


/*static*/ comp_id_type CompVisualSqTemplate::mComponentID = "CompVisualSq" ;
/*static*/ comp_id_type CompVisualSqTemplate::mFamilyID = "CompVisualSq" ;

void CompVisualSqTemplate::Deserialize(rapidxml::xml_node<>* node)
{
    if (node->first_node("sprite"))
    {
        std::string c2 = node->first_node("sprite")->first_attribute("value")->value();
        sprite=atoi(c2.c_str());

        printf("Loading sprite #");
        printf(c2.c_str());
        printf("\n");
    };

};

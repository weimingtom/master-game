
#include "CompTogglable.hpp"
#include "CompTogglableTemplate.hpp"

//#include <cstring>
#include <vector>
#include "../../textUtils.hpp"



/*static*/ comp_id_type CompTogglable::mFamilyID = "CompTogglable";
/*static*/ comp_id_type CompTogglable::mComponentID = "CompTogglable";


CompTogglable::CompTogglable(CompTogglableTemplate *templ) {
	// Construct
	state=templ->state;

}


rapidxml::xml_node<>* CompTogglable::Serialize(xmlFile& doc)
{


    rapidxml::xml_node<> *entityNode = doc.allocate_node(rapidxml::node_element,"CompTogglable");

    static char buffer1 [33];

    sprintf(buffer1, "%d", state);
    //itoa(state,buffer1,10);

    rapidxml::xml_attribute<> *posAttr = doc.allocate_attribute("state", buffer1);

    entityNode->append_attribute(posAttr);


    return entityNode;
};


void CompTogglable::Deserialize(rapidxml::xml_node<>* node)
{

    if (node->first_node("state"))
    {
        std::string c2 = node->first_node("state")->first_attribute("value")->value();
        state=atoi(c2.c_str());
    };


};



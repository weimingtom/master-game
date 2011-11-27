
#include "CompPhysTemplate.hpp"
#include <cstring>



/*static*/ comp_id_type CompPhysTemplate::mComponentID = "CompPhys" ;
/*static*/ comp_id_type CompPhysTemplate::mFamilyID = "CompPhys" ;

rapidxml::xml_node<>* CompPhysTemplate::Serialize(xmlFile& doc)
{

    //запишем новый

    std::string id;


    rapidxml::xml_node<> *entityNode = doc.allocate_node(rapidxml::node_element,"CompPhys");

    static char buffer1 [33];
    static char buffer2 [33];

    itoa(pos.first,buffer1,10);
    itoa(pos.second,buffer2,10);

    strcat(buffer1,",");
    strcat(buffer1,buffer2);

    rapidxml::xml_attribute<> *posAttr = doc.allocate_attribute("pos", buffer1);

    entityNode->append_attribute(posAttr);


    return entityNode;
};


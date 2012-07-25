#include "Component.hpp"
#include "../entity.hpp"
#include <sstream>

std::map<comp_id_type,Component*> gameComponentsTable;

Component::~Component() {};

Component::Component(): mOwnerObj(0)
{
    std::string s1 = static_cast<std::ostringstream*>( &(std::ostringstream() << getUniqueId()) )->str();
    //std::string s1= std::string(getUniqueId());
    id=s1;
    gameComponentsTable[s1]=this;
    std::cout<<"\n" <<"Created component "<< s1;
};


void Component::Deserialize(rapidxml::xml_node<>* node)
{
    if (node->first_node("id"))
    {
        id=node->first_node("id")->first_attribute("value")->value();
        std::cout<<"\n" <<"Created component "<< id;
        gameComponentsTable[id]=this;
    }

};

/*

std::string Component::getField(std::string name)
{
    return table[name];
};

std::string addField (std::string name, std::string value)
{
    table.insert(pair<std::string,std::string>(name,value));
};

std::string removeField (std::string name)
{
    table.erase(name);
};

std::string clearTable()
{
    table.clear();
};

*/

#ifndef __CompNetwork_HPP__
#define __CompNetwork_HPP__

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "Component.hpp"
#include <map>
using namespace std;

typedef std::pair<short,short> vertex_tuple;

class CompNetworkTemplate;

class CompNetwork : public Component {
public:


    multimap<obj_id_type,obj_id_type> network_map;
    typedef multimap<obj_id_type,obj_id_type>::iterator net_iterator;
    typedef pair<multimap<obj_id_type,obj_id_type>::iterator,multimap<obj_id_type,obj_id_type>::iterator> net_element_iterator;

    virtual rapidxml::xml_node<>* Serialize(xmlFile& doc);
	virtual void Deserialize(rapidxml::xml_node<>* node);

    virtual const comp_id_type& componentID() const { return mComponentID; }
	virtual const comp_id_type& familyID() const { return mFamilyID; };

	CompNetwork() {};

	//special constructor for component-template
	CompNetwork(CompNetworkTemplate *templ);



private:

    static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};
#endif

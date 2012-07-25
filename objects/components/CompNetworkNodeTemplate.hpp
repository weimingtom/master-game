
#ifndef __COMPNETWORKNODETEMPLATE_HPP__
#define __COMPNETWORKNODETEMPLATE_HPP__

#include "ComponentTemplate.hpp"
#include "CompNetworkNode.hpp"
#include <cstdio>

#include <iostream>
#include <map>
using namespace std;

class CompNetworkNodeTemplate : public ComponentTemplate {

	// ComponentTemplate interface
public:

    //multimap<obj_id_type,obj_id_type> network_map;
    //typedef multimap<obj_id_type,obj_id_type>::iterator net_iterator;
    //typedef pair<multimap<obj_id_type,obj_id_type>::iterator,multimap<obj_id_type,obj_id_type>::iterator> net_element_iterator;
	//returns the Component ID that this template can create
	virtual const comp_id_type& componentID() const {
		return mComponentID;
	}
	virtual const comp_id_type& familyID() const {
		return mFamilyID;
	}

	virtual Component *makeComponent() {
		CompNetworkNode *comp = new CompNetworkNode(this);
		printf("\n creating CompNetworkNode \n");

		//comp->reset();
		return comp;
	}
    void Deserialize(rapidxml::xml_node<>* node);


	// CompNetwork interface
public:

    comp_id_type id;

    comp_id_type networkId;
private:
	static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};

#endif

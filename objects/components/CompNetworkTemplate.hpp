
#ifndef __CompNetworkTEMPLATE_HPP__
#define __CompNetworkTEMPLATE_HPP__

#include "ComponentTemplate.hpp"
#include "CompNetwork.hpp"
#include <cstdio>

#include <iostream>
#include <map>
using namespace std;

class CompNetworkTemplate : public ComponentTemplate {

	// ComponentTemplate interface
public:

    multimap<comp_id_type,comp_id_type> network_map;
    typedef multimap<comp_id_type,comp_id_type>::iterator net_iterator;
    typedef pair<multimap<comp_id_type,comp_id_type>::iterator,multimap<comp_id_type,comp_id_type>::iterator> net_element_iterator;

	//returns the Component ID that this template can create
	virtual const comp_id_type& componentID() const {
		return mComponentID;
	}
	virtual const comp_id_type& familyID() const {
		return mFamilyID;
	}

	virtual Component *makeComponent() {
		CompNetwork *comp = new CompNetwork(this);
		printf("\n creating CompNetwork \n");

		//comp->reset();
		return comp;
	}
    void Deserialize(rapidxml::xml_node<>* node);


	// CompNetwork interface
public:



private:
	static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};

#endif

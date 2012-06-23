
#ifndef __CompNetworkTEMPLATE_HPP__
#define __CompNetworkTEMPLATE_HPP__

#include "ComponentTemplate.hpp"
#include "CompNetwork.hpp"
#include <cstdio>

class CompNetworkTemplate : public ComponentTemplate {

	// ComponentTemplate interface
public:
	//returns the Component ID that this template can create
	virtual const comp_id_type& componentID() const {
		return mComponentID;
	}
	virtual const comp_id_type& familyID() const {
		return mFamilyID;
	}

	virtual Component *makeComponent() {
		CompNetwork *comp = new CompNetwork(this);
		printf("\n creating CompNetwork %i,%i \n",comp->pos.first,comp->pos.second);

		//comp->reset();
		return comp;
	}
    void Deserialize(rapidxml::xml_node<>* node);


	// CompNetwork interface
public:

    vertex_tuple pos;  // положение в пространстве
    int passable; // проходимо ли

private:
	static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};

#endif

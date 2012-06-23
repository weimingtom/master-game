
#ifndef __COMPPHYSTEMPLATE_HPP__
#define __COMPPHYSTEMPLATE_HPP__

#if defined __MY_UNIX__
#include "ComponentTemplate.hpp"
#endif
#if defined __MY_WIN32__
#include "../ComponentTemplate.hpp"
#endif

#include "CompPhys.hpp"
#include <cstdio>

class CompPhysTemplate : public ComponentTemplate {

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
		CompPhys *comp = new CompPhys(this);
		printf("\n creating CompPhys %i,%i \n",comp->pos.first,comp->pos.second);

		//comp->reset();
		return comp;
	}
    void Deserialize(rapidxml::xml_node<>* node);


	// CompPhys interface
public:

    vertex_tuple pos;  // положение в пространстве
    int passable; // проходимо ли

private:
	static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};

#endif

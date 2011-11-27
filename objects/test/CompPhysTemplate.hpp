
#ifndef __COMPPHYSTEMPLATE_HPP__
#define __COMPPHYSTEMPLATE_HPP__

#include "ComponentTemplate.hpp"
#include "CompPhys.hpp"

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
		//comp->reset();
		return comp;
	}
    rapidxml::xml_node<>* Serialize(xmlFile& doc);


	// CompPhys interface
public:

    vertex_tuple pos;  // положение в пространстве

private:
	static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};

#endif

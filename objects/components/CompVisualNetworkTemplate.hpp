
#ifndef __COMPVISUALNETWORKTEMPLATE_HPP__
#define __COMPVISUALNETWORKTEMPLATE_HPP__

#include "ComponentTemplate.hpp"
#include "CompVisualNetwork.hpp"
#include "../sprites.hpp"


class CompVisualNetworkTemplate : public ComponentTemplate {

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
		CompVisualNetwork *comp = new CompVisualNetwork(this);
		//comp->reset();
		return comp;
	}

	// CompVisualSq interface

	void Deserialize(rapidxml::xml_node<>* node);

public:
	int sprite;

private:

	static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};

#endif

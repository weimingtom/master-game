
#ifndef __COMPVISUALSQTEMPLATE_HPP__
#define __COMPVISUALSQTEMPLATE_HPP__

#if defined __MY_UNIX__
#include "ComponentTemplate.hpp"
#include "../sprites.hpp"
#endif
#if defined __MY_WIN32__
#include "../ComponentTemplate.hpp"
#include "../../sprites.hpp"
#endif

#include "CompVisualSq.hpp"



class CompVisualSqTemplate : public ComponentTemplate {

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
		CompVisualSq *comp = new CompVisualSq(this);
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

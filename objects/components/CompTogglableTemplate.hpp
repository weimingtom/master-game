
#ifndef __COMPTOGGLTEMPLATE_HPP__
#define __COMPTOGGLTEMPLATE_HPP__


#if defined __MY_UNIX__
#include "ComponentTemplate.hpp"
#endif
#if defined __MY_WIN32__
#include "../ComponentTemplate.hpp"
#endif


#include "CompTogglable.hpp"
#include <cstdio>

class CompTogglableTemplate : public ComponentTemplate {

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
		CompTogglable *comp = new CompTogglable(this);
		printf("\n creating CompTogglable \n");

		//comp->reset();
		return comp;
	}
    void Deserialize(rapidxml::xml_node<>* node);


	// CompTogglable interface
public:

    int state;  // состояние

private:
	static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};

#endif


#ifndef __COMPVISUALSQTEMPLATE_HPP__
#define __COMPVISUALSQTEMPLATE_HPP__

#include "ComponentTemplate.hpp"
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


private:

	static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};

#endif

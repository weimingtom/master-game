#ifndef __COMPVISUALHPP__
#define __COMPVISUALHPP__


#include "Component.hpp"

class CompVisual : public Component {
	//Component interface
public:
    //CompVisual interface
	virtual void Draw(){};
	virtual const comp_id_type& familyID() const { return mFamilyID; }
     //CompVisual();
    //~CompVisual();



private:
	static comp_id_type mFamilyID;
};
#endif

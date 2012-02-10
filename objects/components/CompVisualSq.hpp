#ifndef __COMPVISUALSQHPP__
#define __COMPVISUALSQHPP__


#include "CompVisual.hpp"
#include <stdio.h>
class CompVisualSqTemplate;

class CompVisualSq : public CompVisual {
	//Component interface
public:
	virtual const comp_id_type& componentID() const { return mFamilyID; }
	virtual const comp_id_type& familyID() const { return mComponentID; };

	// CompVisual interface

    virtual void Draw();

	// CompVisualSphere interface
public:

	int sprite;

	CompVisualSq() {};

		//special constructor for component-template
	CompVisualSq(CompVisualSqTemplate *templ);

    virtual rapidxml::xml_node<>* Serialize(xmlFile& doc);
    virtual void Deserialize(rapidxml::xml_node<>* node);


private:
	static comp_id_type mFamilyID;
	static comp_id_type mComponentID;
};



#endif

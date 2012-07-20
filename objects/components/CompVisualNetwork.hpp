#ifndef __COMPVISUALNETWORKHPP__
#define __COMPVISUALNETWORKHPP__


#include "CompVisual.hpp"
#include <stdio.h>
class CompVisualNetworkTemplate;

class CompVisualNetwork : public CompVisual {
	//Component interface
public:
	virtual const comp_id_type& componentID() const { return mComponentID; }
	virtual const comp_id_type& familyID() const { return mFamilyID; };

	// CompVisual interface

    virtual void Draw();

	// CompVisualSphere interface
public:

	int sprite;

	CompVisualNetwork() {};

		//special constructor for component-template
	CompVisualNetwork(CompVisualNetworkTemplate *templ);

    virtual rapidxml::xml_node<>* Serialize(xmlFile& doc);
    virtual void Deserialize(rapidxml::xml_node<>* node);


private:
	static comp_id_type mFamilyID;
	static comp_id_type mComponentID;
};



#endif

#ifndef __COMPPHYS_HPP__
#define __COMPPHYS_HPP__

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"



#if defined __MY_UNIX__
#include "Component.hpp"
#endif
#if defined __MY_WIN32__
#include "../Component.hpp"
#endif

typedef std::pair<short,short> vertex_tuple;

class CompPhysTemplate;

class CompPhys : public Component {
public:

    vertex_tuple pos;  // положение в пространстве
    int passable;  // проходимо ли

    virtual rapidxml::xml_node<>* Serialize(xmlFile& doc);
	virtual void Deserialize(rapidxml::xml_node<>* node);

    virtual const comp_id_type& componentID() const { return mComponentID; }
	virtual const comp_id_type& familyID() const { return mFamilyID; };

	CompPhys() {};

	//special constructor for component-template
	CompPhys(CompPhysTemplate *templ);



private:

    static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};
#endif

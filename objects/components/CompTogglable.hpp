
#ifndef __COMPTOGGL_HPP__
#define __COMPTOGGL_HPP__

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"


#if defined __MY_UNIX__
#include "Component.hpp"
#endif
#if defined __MY_WIN32__
#include "../Component.hpp"
#endif


class CompTogglableTemplate;

class CompTogglable : public Component {
public:

    int state;  // состояние кнопки

    virtual rapidxml::xml_node<>* Serialize(xmlFile& doc);
	virtual void Deserialize(rapidxml::xml_node<>* node);

    virtual const comp_id_type& componentID() const { return mComponentID; }
	virtual const comp_id_type& familyID() const { return mFamilyID; };

	CompTogglable() {};

	//special constructor for component-template
	CompTogglable(CompTogglableTemplate *templ);



private:

    static comp_id_type mComponentID;
	static comp_id_type mFamilyID;
};
#endif

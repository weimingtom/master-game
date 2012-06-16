
#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <string>
#include <map>
#include <cstring>

#include "Component.hpp"

#if defined __MY_WIN32__
#define not !
#endif



typedef std::map<const comp_id_type, Component*> component_table_type;

typedef std::string obj_id_type;

class Object {
public:


	Object( const obj_id_type& id) {
		setID(id);
	}

	const obj_id_type& getID() const { return mOID; }
	void setID( const obj_id_type& id ) { mOID = id; }

	Component* getComponent(const comp_id_type& familyID ) {
		return mComponents[familyID];
	}
	Component* setComponent(Component *newComp) {
		comp_id_type family = newComp->familyID();
		newComp->setOwnerObject(this);
		Component *oldComp = mComponents[family];
		mComponents[family] = newComp;
		return oldComp;
	}
	void clearComponents() {
		component_table_type::iterator iter;
		for(iter = mComponents.begin(); iter != mComponents.end(); iter++) {
			delete iter->second;
		}
		mComponents.clear();
	}

    component_table_type* Components() {
		return &mComponents;
	}

	void Deserialize(rapidxml::xml_node<>* node)
	{
        // компоненты
        component_table_type::iterator iter;
		for(iter = mComponents.begin(); iter != mComponents.end(); iter++)
		{

            rapidxml::xml_node<char> *templ = node->first_node("components")->first_node("goc");

            while (templ)
            {

                if (not (strcmp(templ->first_attribute("name")->value(),iter->second->componentID().c_str())))
                    {
                        iter->second->Deserialize(templ);
                    }

                templ=templ->next_sibling("goc");
            }
		}

    }

	rapidxml::xml_node<>* Serialize(xmlFile& doc)
	{

	    rapidxml::xml_node<> *out = doc.allocate_node(rapidxml::node_element,"components");


        component_table_type::iterator iter;
		for(iter = mComponents.begin(); iter != mComponents.end(); iter++)
		{
		    out->append_node(iter->second->Serialize(doc));

		}
    return out;
    };

private:
	obj_id_type mOID;			//unique identifier for this object
	component_table_type mComponents;		//map of all components;
};

#endif

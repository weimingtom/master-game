
#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <string>
#include <map>
#include <cstring>

#include "Component.hpp"

typedef std::map<const comp_id_type, Component*> component_table_type;

typedef std::string obj_id_type;

class Object {
public:


	Object( const obj_id_type& id) {
		setID(id);
	}

	const obj_id_type& getID() const { return mOID; }
	void setID( const obj_id_type& id ) { mOID = id; }

	Component* getComponent(const comp_id_type& componentID ) {
		return mComponents[componentID];
	}

	component_table_type* getComponentOfFamily(const comp_id_type& familyID ) {
		component_table_type* selected_components = new component_table_type;
		for (component_table_type::iterator i1 =  mComponents.begin();i1 != mComponents.end(); i1++)
		{
		    if (i1->second->familyID()==familyID){
                selected_components->insert(std::pair<const comp_id_type, Component*>(i1->second->componentID(),i1->second));
            }
		}
		return selected_components;
	}


    bool hasComponent(const comp_id_type& componentID ) {
		return not (mComponents.find(componentID)==mComponents.end());
	}

	Component* setComponent(Component *newComp) {
		comp_id_type compID = newComp->componentID();
		newComp->setOwnerObject(this);
		Component *oldComp = mComponents[compID];
		mComponents[compID] = newComp;
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

extern std::map<obj_id_type,Object*> gameObjectsTable;

#endif

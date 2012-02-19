
#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include <string>
#include <map>

class Object;
typedef std::string comp_id_type;
typedef std::string field_id_type;
typedef std::map<field_id_type, std::string> objTable;
typedef rapidxml::xml_document<> xmlFile;
typedef std::pair<short,short> vertex_tuple;



class Component {
public:

	Component() : mOwnerObj(0) {}
	virtual ~Component() = 0;

	virtual const comp_id_type& componentID() const = 0;
	virtual const comp_id_type& familyID() const = 0;

	virtual void update() {};
    virtual rapidxml::xml_node<>* Serialize(xmlFile& doc) =0 ;
	virtual void Deserialize(rapidxml::xml_node<>* node) =0;


	void setOwnerObject( Object* obj) { mOwnerObj = obj; }
	Object* getOwnerObject() const { return mOwnerObj; }

    /*std::string getField (std::string name);
    std::string addField (std::string name, std::string value);
    std::string removeField (std::string name);
    std::string clearTable();
    */


private:
	Object *mOwnerObj;
    objTable table;
};



#endif

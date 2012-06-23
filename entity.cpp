
#if defined __MY_UNIX__
#include "Object.hpp"
#include "components/CompPhys.hpp"
#endif
#if defined __MY_WIN32__
#include "objects/Object.hpp"
#include "objects/components/CompPhys.hpp"
//#include <set>
#endif


std::map<obj_id_type,Object*> gameObjectsTable;

#if defined __MY_UNIX__
int getUniqueId()
{
    static std::set<int> idTable;
    int id1=rand();
    while (idTable.find(id1)!=idTable.end())
        id1=rand();

    idTable.insert(id1);
    return id1;
};


std::vector<Object*> getObjectsWithCoords(int x, int y)
{
    std::vector<Object*> result;
    for (std::map<std::string,Object*>::iterator go1=gameObjectsTable.begin();go1!=gameObjectsTable.end();go1++)
    {
        //std::string p1 = go1->second->getComponent("CompPhys")->getField("pos");

        //int x1,y1;

        //x1=split(p1)

        //std::stringstream st1;
        //st1 << x <<","<<y;
        vertex_tuple p1;

        p1=static_cast<CompPhys*>(go1->second->getComponent("CompPhys"))->pos;

        if ((p1.first==x)&&(p1.second==y))
        {
            result.push_back(go1->second);
        }
    };
    return result;
}
#endif


#if defined __MY_WIN32__
using namespace std;

#include <set>
#include <vector>



int getUniqueId()
{
	static set<int> idTable;
    int id1=rand();
    while (idTable.find(id1)!=idTable.end())
        id1=rand();

    idTable.insert(id1);
    return id1;

};


vector<Object*> getObjectsWithCoords(int x, int y)
{
    vector<Object*> result;
    for (map<string,Object*>::iterator go1=gameObjectsTable.begin();go1!=gameObjectsTable.end();go1++)
    {
        //std::string p1 = go1->second->getComponent("CompPhys")->getField("pos");

        //int x1,y1;

        //x1=split(p1)

        //std::stringstream st1;
        //st1 << x <<","<<y;
        vertex_tuple p1;

        p1=static_cast<CompPhys*>(go1->second->getComponent("CompPhys"))->pos;

        if ((p1.first==x)&&(p1.second==y))
        {
            result.push_back(go1->second);
        }
    };
    return result;

}


#endif
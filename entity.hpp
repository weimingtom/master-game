#ifndef __ENTITY_INCLUDED__
#define __ENTITY_INCLUDED__

#include <vector>
#include <set>
#include <map>
#include "math.h"
#include "globals.hpp"
#include <stdlib.h>

#include "textUtils.hpp"

#include "Object.hpp"


extern std::map<obj_id_type,Object*> gameObjectsTable;

int getUniqueId();
std::vector<Object*> getObjectsWithCoords(int x, int y);




#endif


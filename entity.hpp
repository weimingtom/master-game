#ifndef __ENTITY_INCLUDED__
#define __ENTITY_INCLUDED__

#include <vector>
#include <set>
#include <map>
#include "math.h"
#include "globals.hpp"
#include <stdlib.h>

#include "textUtils.hpp"

#if defined __MY_UNIX__
#include "Object.hpp"
#endif
#if defined __MY_WIN32__
#include "objects\Object.hpp"
#endif

extern std::map<obj_id_type,Object*> gameObjectsTable;

int getUniqueId();
std::vector<Object*> getObjectsWithCoords(int x, int y);




#endif


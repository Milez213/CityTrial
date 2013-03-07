
#ifndef LOAD_MAP_H
#define LOAD_MAP_H


#include "GameBuilding.h"

#include <set>


bool loadMap(const char* filename, std::set<GameDrawableObject* > &map);

#endif


#pragma once
#include "../ai/ai.h"

#include <vector>

void generateMap(int wallDensity,
                 int energyDensity,
                 int numberActors,
                 int xSize,
                 int ySize,
                 int energyMin,
                 int energyMax,
                 std::vector<Agent> agents);

unsigned char *map;
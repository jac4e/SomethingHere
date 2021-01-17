#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

void generateMap(int wallDensity, int energyDensity, int numberActors, int xSize, int ySize, int energyMin, int energyMax, std::vector<Agent> agents);

unsigned char *map;

#endif // MAPGENERTOR_H

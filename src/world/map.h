#pragma once
#include <vector>
#include "../ai/agent.h"
#include "position.h"


// Encapsulate map stuff
class Map {
   public:
    unsigned char *map;
    int dimension;
    int maxLength;
    Map();
    int width;
    int height;
    int wallDensity;
    int energyDensity;
    int energyMin;
    int energyMax;
    unsigned char *&getMap();
    unsigned char getCell(Position position);
    void setCell(Position position, unsigned char type);
    void clear();
    void generate(int dim, int mEmpty, int mLen, int eDen, int eMin, int eMax);
    void placeEnergy();
    void generateWalls();

    void generateWallsImproved();

    void print();
    void randomize();
    void placeAgents(std::vector<Agent> &population);
    // Analogous to getAdjacent found in 
    std::vector<unsigned char> getCellRadius(Position position, int radius);

    Map(int w, int h, int wD, int eD, int eMin, int eMax);;
};

void printMap(std::vector<unsigned char> map);
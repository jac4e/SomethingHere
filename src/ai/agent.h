#pragma once
#include "../world/position.h"

class Agent {
   public:
    Position pos;
    // Movement weights [empty, agent, wall, energy]
    std::vector<float> moveWeights;
    std::vector<float> visionWeights;

    unsigned long long int id;
    int radius;
    int energyUsed;
    int energyConsumed;
    int energyStorage;
    int deathTime;
    float litness;
    int maxEnergy;
    float selectionProbability;
    Agent(std::vector<float> moveW, std::vector<float> visionW, int radius);
    void reset();
    void setPosition(int x, int y);
    void setProperties(std::vector<float> moveW, std::vector<float> visionW);
    int stealEnergy(int amt);
    int useEnergy(int amt);
    void kill(int i);
};
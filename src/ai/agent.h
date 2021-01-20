#pragma once
#include "../world/position.h"

class Agent {
   public:
    Position pos;
    // Movement weights [empty, agent, wall, energy]
    std::vector<float> moveWeights;
    unsigned long long int id;
    float stge, str;
    int radius;
    int energyUsed;
    int energyConsumed;
    int energyStorage;
    int deathTime;
    float litness;
    int maxEnergy;
    float selectionProbability;
    Agent(std::vector<float> w, float a, float b);
    void reset();
    void setPosition(int x, int y);
    void setProperties(std::vector<float> w, float stge, float str);
    int stealEnergy(int amt);
    int useEnergy(int amt);
    void kill(int i);
};
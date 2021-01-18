#pragma once
#include "../world/position.h"

class Agent {
   public:
    Position pos;
    // Movement weights [empty, agent, wall, energy]
    std::vector<float> moveWeights;
    float stge, str;
    int radius;
    int energyUsed;
    int energyStorage;
    int deathTime;
    float litness;
    int maxEnergy;
    float selectionProbability;
    Agent();
    void setPosition(int x, int y);
    void setProperties(std::vector<float> w, float stge, float str, int r);
    int stealEnergy(int amt);
    int useEnergy(int amt);
    void kill();
};
#include <algorithm>

#include "../world/map.h"
#include "ai.h"

extern Map map;
extern int time_count;
extern int deaths;

unsigned long long int counter;

Agent::Agent(std::vector<float> moveW, std::vector<float> mapW, int rad) {
    id = counter;
    ++counter;
    moveWeights = moveW;
    visionWeights = mapW;
    litness = 0;
    radius = rad;
    energyUsed = 0;
    pos = {0, 0};
    energyConsumed = 0;
    deathTime = 0;
    maxEnergy = 500;
    energyStorage = maxEnergy / 2;
    selectionProbability = 0;
};

void Agent::setPosition(int x, int y) {
    pos = {x, y};
    return;
};

void Agent::setProperties(std::vector<float> moveW, std::vector<float> visionW) {
    moveWeights = moveW;
    visionWeights = visionW;
    energyStorage = maxEnergy / 2;
}

int Agent::stealEnergy(int amt) {
    if (energyStorage > amt) {
        energyStorage -= amt;
        return amt;
    } else {
        int taken = energyStorage;
        energyStorage = 0;
        return taken;
    }
}
int Agent::useEnergy(int amt) {
    // returns 1 when energy is used
    // returns 0 when energy demand exceeds storage
    if (energyStorage < amt) {
        energyUsed += energyStorage;
        energyStorage = 0;
        return 0;
    } else {
        energyStorage -= amt;
        energyUsed += amt;
    }
}

std::vector<int> deathList;

void Agent::kill(int i) {
    // kill agent when energy runs out or too much energy consumed
    if (deaths >= 1000) {
        int tst = 0;
    }
    if (std::find(deathList.begin(), deathList.end(), i) != deathList.end()) {
        int tst = 0;
    }
    ++deaths;
    //printf("Agent %llu died, they were the %d death\n",id, deaths);
    deathTime = time_count;
    map.setCell(pos, 0);
    deathList.push_back(i);
}

void Agent::reset() {
    litness = 0;
    pos = {0, 0};
    energyUsed = 0;
    energyConsumed = 0;
    energyStorage = maxEnergy / 2;
    deathTime = 0;
    selectionProbability = 0;
}

std::vector<Agent> generateAgents(int amt, int radius) {
    std::vector<Agent> population;

    for (int i = 0; i < amt; i++) {
        // Generate floats between 0 and 1 by calculating the normal of rand()
        std::vector<float> moveWeights = {nRand(), nRand(), nRand(), nRand()};
        std::vector<float> visionWeights;
        for (int y = 0; y < radius * 2 + 1; y++) {
            for (int x = 0; x < radius * 2 + 1; x++) {
                visionWeights.push_back(nRand());
            }
        }

        // Set the properties based on the randomly generated values
        Agent agent(moveWeights, visionWeights, radius);
        population.push_back(agent);
    }
    return population;
}

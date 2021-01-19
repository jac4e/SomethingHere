#include "../world/map.h"
#include "ai.h"


extern Map map;
extern int time;
extern int deaths;

Agent::Agent(){
    moveWeights = { 0,0,0,0 };
    stge = 0;
    str = 0;
    litness = 0;
    radius = 0;
    energyUsed = 0;
    energyStorage = 0;
    deathTime = 0;
    maxEnergy = 0;
    selectionProbability = 0;
};

void Agent::setPosition(int x, int y) {
    pos = {x, y};
    return;
};

void Agent::setProperties(std::vector<float> w, float a, float b, int r) {
    moveWeights = w;
    stge = a;
    deathTime = 0;
    str = b;
    radius = r;
    maxEnergy = 100 * stge;
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
int Agent::useEnergy(int amt){
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

void Agent::kill() {
    // kill agent when energy runs out or too much energy consumed
    ++deaths;
    deathTime = time;
    map.setCell(pos,0);
}

void Agent::reset() {
    litness = 0;
    energyUsed = 0;
    energyStorage = maxEnergy / 2;
    deathTime = 0;
    selectionProbability = 0;
}

std::vector<Agent> generateAgents(int amt, int skillMax) {
    std::vector<Agent> population(amt);

    for (int i = 0; i < amt; i++) {
        // Generate floats between 0 and 1 by calculating the normal of rand()
        std::vector<float> moveWeights = {nRand(), nRand(), nRand(), nRand()};

        // Generate two numbers that sum to skillMax by randomly generating two cut-off points
        int a;
        do {
            a = rand() % skillMax + 1;
        } while (a >= skillMax);

        float stge = (float)a;
        float str = skillMax - (float)a;

        // Set the properties based on the randomly generated values
        population[i].setProperties(moveWeights, stge, str, 4);
    }
    return population;
}
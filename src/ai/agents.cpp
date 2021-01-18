#include "../world/map.h"
#include "ai.h"

Agent::Agent(){};


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

void Agent::stealEnergy(int amt){
    energyStorage -= amt;
}

void Agent::kill() {
    // kill agent when energy runs out or too much energy consumed
    int index = (pos.y * rowsize) + pos.x;
    map[index] = 0;
}

std::vector<Agent> generateAgents(int amt, int skillMax) {
    std::vector<Agent> population(amt);

    for (int i = 0; i < amt; i++) {
        // Generate floats between 0 and 1 by calculating the normal of rand()
        std::vector<float> moveWeights = {nRand(),nRand(),nRand(),nRand()};

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
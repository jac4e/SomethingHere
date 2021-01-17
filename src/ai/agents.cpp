#include "ai.hpp"
#include "../RandomGeneration/mapGenerator.h"

Agent::Agent(){};

void Agent::setPosition(int x, int y) {
    pos = {x, y};
    return;
};

void Agent::setProperties(std::vector<float> w, float a, float b) {
    moveWeights = w;
    stge = a;
    str = b;
    radius = 2;
    maxEnergy = 100 * stge;
    energyStorage = maxEnergy/2;
}

void Agent::stealEnergy(int amt){
    energyStorage -= amt;
}

void Agent::kill(int rowsize){
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
        population[i].setProperties(moveWeights, stge, str);
    }
    return population;
}
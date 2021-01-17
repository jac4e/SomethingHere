#include "ai.hpp"

Agent::Agent(){};

void Agent::setPosition(int x, int y) {
    pos = {x, y};
    return;
};

void Agent::setProperties(float a, float b, float c, float d, float e) {
    agg = a;
    picky = b;
    stge = c;
    str = d;
    agi = e;
    fitness = rand();
}

void Agent::calculateFitness() {
    fitness = rand();
}

std::vector<Agent> generateAgents(int amt, int skillMax) {
    std::vector<Agent> population(amt);

    for (int i = 0; i < amt; i++) {
        // Generate floats between 0 and 1 by calculating the normal of rand()
        float agg = (float)rand() / (float)RAND_MAX;
        float picky = (float)rand() / (float)RAND_MAX;

        // Generate three numbers that sum to skillMax by randomly generating two cut-off points
        int a;
        int b;
        do {
            a = rand() % skillMax + 1;
            b = rand() % skillMax + 1;
        } while (a >= b || b >= skillMax);

        float stge = (float)a;
        float str = (float)b - (float)a;
        float agi = (float)skillMax - (float)b;

        // Set the properties based on the randomly generated values
        population[i].setProperties(agg, picky, stge, str, agi);
    }
    return population;
}
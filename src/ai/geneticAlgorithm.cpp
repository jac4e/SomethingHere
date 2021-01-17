#include "ai.hpp"

void assignProbabilities(std::vector<Agent> &population) {
    // First sum all fitness values
    int fitnessSum;
    for (int i = 0; i < population.size(); i++) {
        fitnessSum += population[i].energyUsed;
    }
    // Then calculate probability for each agent
    float probSum;
    for (int i = 0; i < population.size(); i++) {
        probSum += (float)population[i].energyUsed / (float)fitnessSum;
        population[i].selectionProbability = probSum;
        // printf("prob: %f\n", population[i].selectionProbability);
    }
}

int selectParent(std::vector<Agent> &population) {
    float r = (float)rand() / (float)RAND_MAX;
    for (int i = 0; i < population.size(); i++) {
        // printf("size: %d", population.size());
        if (i == population.size() - 1 || (r > population[i].selectionProbability && r < population[i + 1].selectionProbability)) {
            return i;
        }
    }
};
void mutate(Agent &agent) {
    ;
    // Mutate Movement Weights;
    std::vector<float> moveWeights;
    for (int i = 0; i < agent.moveWeights.size(); i++) {
        float lMax = agent.moveWeights[i];
        float rMax = 1 - agent.moveWeights[i];
        float shift = (nRand() * (lMax + rMax) - lMax) / 10;
        moveWeights.push_back(agent.moveWeights[i] + shift);
    }

    // Mutate Physical
    // Calculate the max cutoff shift
    float aMax[2] = {agent.stge, agent.str};
    // Calculate normalized random numbers
    float arand = (float)rand() / (float)RAND_MAX;
    // Calculate cuttoff point shift value
    float aShift = (arand * (aMax[0] + aMax[1]) - aMax[0]) / 10;
    float stge = agent.stge + aShift;
    float str = agent.str - aShift;
    agent.setProperties(moveWeights, stge, str);
}

Agent breed(Agent &parentA, Agent &parentB, float w) {
    // Breeds a child of two parents
    // Child properties are determined by a weight
    // Should modify this after first initial simulations if improper converging occurs
    Agent child;

    std::vector<float> moveWeights;
    for (int i = 0; i < parentA.moveWeights.size(); i++) {
        moveWeights.push_back(parentA.moveWeights[i] * w + parentB.moveWeights[i] * (1 - w));
    }

    child.setProperties(
        moveWeights,
        parentA.stge * w + parentB.stge * (1 - w),
        parentA.str * w + parentB.str * (1 - w));
    mutate(child);
    return child;
}

void reproducePopulation(std::vector<Agent> &population) {
    // Assign probabilities
    assignProbabilities(population);

    // Select mating population through stochastic universal sampling
    std::vector<Agent> matingPop;
    int matingPopSize = population.size() / 2;
    float pointerDist = 1 / (float)matingPopSize;
    float start = pointerDist * ((float)rand() / (float)RAND_MAX);
    // printf("matingPopSize: %d\npointerDist: %f\nstart: %f\n", matingPopSize, pointerDist, start);
    int i = 0;
    for (int j = 0; j < population.size(); j++) {
        float pointer = start + pointerDist * i;
        // printf("i: %d\nj: %d\npointer: %f\nprob: %f\n",i,j,pointer,population[j].selectionProbability);
        if (pointer < population[j].selectionProbability) {
            matingPop.push_back(population[j]);
            ++i;
        }
    }
    // Clear population
    population.clear();
    // printf("matingPop: %d\n",matingPopSize);
    // Reassign probabilities for mating population
    assignProbabilities(matingPop);

    // Breed mating population until matingPop is empty
    while (matingPop.size() > 0) {
        // Select parents and remove them from matingPop
        int a = selectParent(matingPop);
        Agent parentA = matingPop[a];
        matingPop.erase(matingPop.begin() + a);
        int b = selectParent(matingPop);
        Agent parentB = matingPop[b];
        matingPop.erase(matingPop.begin() + b);

        // Breed Children and add to population
        float w = (float)rand() / (float)RAND_MAX;
        population.push_back(breed(parentA, parentB, w));
        population.push_back(breed(parentB, parentA, 1 - w));

        // Add parents back to population
        population.push_back(parentA);
        population.push_back(parentB);
    }
}
#include "ai.hpp"

void assignProbabilities(std::vector<Agent> &population) {
    // First sum all fitness values
    int fitnessSum = NULL;
    for (int i = 0; i < population.size(); i++) {
        fitnessSum += population[i].fitness;
    }
    // Then calculate probability for each agent
    float probSum = NULL;
    for (int i = 0; i < population.size(); i++) {
        probSum += (float)population[i].fitness / (float)fitnessSum;
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
}
;
void mutate(Agent &agent) {;
    // Mutate Personality;
    float agg = agent.agg + ( (float)rand() / (float)RAND_MAX )/10;
    float picky = agent.picky + ( (float)rand() / (float)RAND_MAX )/10;

    // Mutate Physical
    // Calculate the max left and right shift of cutoff point a and b
    float aMax[2] = {agent.stge, agent.str};
    float bMax[2] = {agent.str, agent.agi};
    // Calculate two normalized random numbers
    float arand = (float)rand() / (float)RAND_MAX;
    float brand = (float)rand() / (float)RAND_MAX;
    // Calculate cuttoff point shift value
    float aShift = (arand * (aMax[0] + aMax[1]) - aMax[0]) / 10;
    float bShift = (brand * (bMax[0] + bMax[1]) - bMax[0]) / 10;
    float stge = agent.stge + aShift;
    float str = agent.str - aShift + bShift;
    float agi = agent.agi - bShift;
    agent.setProperties(agg, picky, stge, str, agi);
}

Agent breed(Agent &parentA, Agent &parentB, float crossover) {
    // Breeds a child of two parents from crossover point
    // Cross over point is from 0 to 1
    Agent child;
    child.setProperties(
        parentA.agg * crossover + parentB.agg * (1 - crossover),
        parentA.picky * crossover + parentB.picky * (1 - crossover),
        parentA.stge * crossover + parentB.stge * (1 - crossover),
        parentA.str * crossover + parentB.str * (1 - crossover),
        parentA.agi * crossover + parentB.agi * (1 - crossover));
    mutate(child);
    printf("stge - %f | str - %f | agi - %f\n", child.stge, child.str, child.agi);
    printf("Tot: %f\n", child.stge + child.str + child.agi);
    return child;
}

void reproducePopulation(std::vector<Agent> &population) {
    //calculate fitness
    for (int i = 0; i < population.size(); i++) {
        population[i].calculateFitness();
    }
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
        float crossover = (float)rand() / (float)RAND_MAX;
        population.push_back(breed(parentA, parentB, crossover));
        population.push_back(breed(parentB, parentA, 1 - crossover));

        // Add parents back to population
        population.push_back(parentA);
        population.push_back(parentB);
    }
}
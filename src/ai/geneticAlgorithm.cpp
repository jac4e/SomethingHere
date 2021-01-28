#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ai.h"

void assignProbabilities(std::vector<Agent> &population) {
    // Fitness should be determined
    // First sum all fitness values
    float fitnessSum = 0;
    for (int i = 0; i < population.size(); i++) {
        fitnessSum += population[i].litness;
    }
    // Then calculate probability for each agent
    float probSum = 0;
    for (int i = 0; i < population.size(); i++) {
        probSum += population[i].litness / fitnessSum;
        population[i].selectionProbability = probSum;
        // printf("prob: %f\n", population[i].selectionProbability);
    }
}

int selectParent(std::vector<Agent> &population) {
    float r = (float)rand() / (float)RAND_MAX;
    for (int i = 0; i < population.size(); i++) {
        // printf("size: %d", population.size());
        if (r < population[i].selectionProbability) {
            return i;
        }
    }
};
void mutate(Agent &agent) {
    // Should be mutated based on normal distribution centered on premutated value

    // Mutate Movement Weights;
    std::vector<float> moveWeights;
    for (int i = 0; i < agent.moveWeights.size(); i++) {
        float lMax = agent.moveWeights[i];
        float rMax = 1 - agent.moveWeights[i];
        float shift = (nRand() * (lMax + rMax) - lMax) / 10;
        moveWeights.push_back(agent.moveWeights[i] + shift);
    }
    std::vector<float> visionWeights;
    for (int i = 0; i < agent.visionWeights.size(); i++) {
        float lMax = agent.visionWeights[i];
        float rMax = 1 - agent.visionWeights[i];
        float shift = (nRand() * (lMax + rMax) - lMax) / 10;
        moveWeights.push_back(agent.visionWeights[i] + shift);
    }

    agent.setProperties(moveWeights, visionWeights);
}

std::vector<Agent> breed(Agent &parentA, Agent &parentB) {
    // Breeds a child of two parents
    // Should modify this after first initial simulations if improper converging occurs
    std::vector<Agent> children;
    std::vector<std::vector<float>> childMoveWeights(2, std::vector<float>(4));
    int dia = parentA.radius * 2 + 1;
    std::vector<std::vector<float>> childVisionWeights(2, std::vector<float>(dia * dia));
    // randomly inheriet either parents moveWeights
    for (int i = 0; i < parentA.moveWeights.size(); i++) {
        int r = rand() % 2;
        childMoveWeights[0][i] = (r) ? parentA.moveWeights[i] : parentB.moveWeights[i];
        childMoveWeights[1][i] = (!r) ? parentA.moveWeights[i] : parentB.moveWeights[i];
    }

    for (int i = 0; i < parentA.visionWeights.size(); i++) {
        int r = rand() % 2;
        childVisionWeights[0][i] = (r) ? parentA.visionWeights[i] : parentB.visionWeights[i];
        childVisionWeights[1][i] = (!r) ? parentA.visionWeights[i] : parentB.visionWeights[i];
    }

    Agent childA(childMoveWeights[0], childVisionWeights[0], parentA.radius);
    Agent childB(childMoveWeights[1], childVisionWeights[1], parentA.radius);

    //mutate(childA);
    //mutate(childB);
    children.push_back(childA);
    children.push_back(childB);

    return children;
}

void calculateLitness(Agent &agent) {
    agent.litness = agent.energyUsed;
}

void reproducePopulation(std::vector<Agent> &population) {
    // Assign probabilities
    assignProbabilities(population);

    // Select mating population through stochastic universal sampling
    std::vector<Agent> matingPop;
    int matingPopSize = population.size() / 2;
    float pointerDist = 1 / (float)matingPopSize;
    float start = pointerDist * ((float)rand() / (float)RAND_MAX);

    std::vector<float> pointersaver;
    for (int i = 0; i < matingPopSize; i++) {
        float pointer = start + pointerDist * i;
        for (size_t j = 0; j < population.size(); j++) {
            if (pointer < population[j].selectionProbability) {
                pointersaver.push_back(pointer);
                matingPop.push_back(population[j]);
                population.erase(population.begin() + j);
                break;
            }
        }
    }
    // On some occasions, one agents normalized fitness is so large, spanning the length of two or more pointers
    // This causes the mating population to be too small
    // Simple fix is to check for mating population size and randomly select additional agents untill mating population is full
    // This is done using the select parent function as it is equivilent to rollette wheel selection
    assignProbabilities(population);
    while (matingPop.size() < matingPopSize) {
        int selected = selectParent(population);
        matingPop.push_back(population[selected]);
    }

    // Clear population
    population.clear();
    // printf("matingPop: %d\n",matingPopSize);

    // Breed mating population until matingPop is empty
    while (matingPop.size() > 0) {
        // Reassign probabilities for mating population
        assignProbabilities(matingPop);
        // Select parents and remove them from matingPop
        //int a = selectParent(matingPop);
        int a = rand() % matingPop.size();
        Agent parentA = matingPop[a];
        matingPop.erase(matingPop.begin() + a);
        //int b = selectParent(matingPop);
        int b = rand() % matingPop.size();
        Agent parentB = matingPop[b];
        matingPop.erase(matingPop.begin() + b);

        // Breed Children and add to population
        std::vector<Agent> children = breed(parentA, parentB);
        population.push_back(children[0]);
        population.push_back(children[1]);

        // Add parents back to population
        parentA.reset();
        parentB.reset();
        population.push_back(parentA);
        population.push_back(parentB);
    }
}
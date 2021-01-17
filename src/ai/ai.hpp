#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>

struct Position {
    int x;
    int y;
};

class Agent {
   public:
    Position pos;
    float agg, picky, stge, str, agi;
    int fitness;
    float selectionProbability;
    Agent();
    void setPosition(int x, int y);
    void setProperties(float agg, float picky, float stge, float str, float agi);
    void calculateFitness();
};

std::vector<Agent> generateAgents(int amt, int skillMax);

void assignProbabilities(std::vector<Agent> &population);
int selectParent(std::vector<Agent> &population);
void mutate(Agent &agent);
Agent breed(Agent &parentA, Agent &parentB, float crossover);
void reproducePopulation(std::vector<Agent> &population);

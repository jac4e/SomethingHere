#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

struct Position {
    int x;
    int y;
};

bool operator==(const Position& left, const Position& right);

class Agent {
   public:
    Position pos;
    // Movement weights [empty, agent, wall, energy]
    std::vector<float> moveWeights;
    float stge, str;
    int radius;
    int energyUsed;
    int energyStorage;
    int maxEnergy;
    float selectionProbability;
    Agent();
    void setPosition(int x, int y);
    void setProperties(std::vector<float> w, float stge, float str);
    void stealEnergy(int amt);
    void kill(int rowsize);
};

float nRand();

std::vector<Agent> generateAgents(int amt, int skillMax);

void assignProbabilities(std::vector<Agent> &population);
int selectParent(std::vector<Agent> &population);
void mutate(Agent &agent);
Agent breed(Agent &parentA, Agent &parentB, float crossover);
void reproducePopulation(std::vector<Agent> &population);

Agent& getAgent(std::vector<Agent> &population, Position pos);
std::vector<char> getAdjacent(char *map, int rowsize, Agent &agent, int radius);
void control(Agent &agent);

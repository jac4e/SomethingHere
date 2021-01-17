#pragma once
#include <vector>

struct Position {
    int x;
    int y;
};

class Agent{
    public:
        Position pos;
        float agg, picky;
        int stge, str, agi;
        Agent();
        void setPos(int, int);
        void setProperties(float,float,int,int,int);
};

std::vector<Agent> generateAgent(int,int);

void assignProbabilities(std::vector<Agent> &population);
int selectParent(std::vector<Agent> &population);
void mutate(Agent &agent);
Agent breed(Agent &parentA, Agent &parentB, float crossover);
void reproducePopulation(std::vector<Agent> &population);

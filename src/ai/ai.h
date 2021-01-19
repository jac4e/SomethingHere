#pragma once
#include <vector>
#include "../world/position.h"
#include "agent.h"

float nRand();

std::vector<Agent> generateAgents(int amt, int skillMax);

void assignProbabilities(std::vector<Agent> &population);
int selectParent(std::vector<Agent> &population);
void mutate(Agent &agent);
Agent breed(Agent &parentA, Agent &parentB, float crossover);
void reproducePopulation(std::vector<Agent> &population);
void calculateLitness(Agent& agent);
Agent &getAgent(std::vector<Agent> &population, Position pos);
void control(Agent &agent, std::vector<Agent> &population);

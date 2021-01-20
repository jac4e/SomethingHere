#pragma once
#include <vector>
#include "../world/position.h"
#include "agent.h"

float nRand();

std::vector<Agent> generateAgents(int amt, int skillMax);

void assignProbabilities(std::vector<Agent> &population);
int selectParent(std::vector<Agent> &population);
void mutate(Agent &agent);
std::vector<Agent> breed(Agent &parentA, Agent &parentB);
void reproducePopulation(std::vector<Agent> &population);
void calculateLitness(Agent& agent);
int getAgent(std::vector<Agent> &population, Position pos, std::vector<unsigned char>& adjacent);
void control(Agent &agent, std::vector<Agent> &population, int i);

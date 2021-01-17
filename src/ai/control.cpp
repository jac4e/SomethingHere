#include "ai.hpp"
#include "../RandomGeneration/mapGenerator.h"

bool operator==(const Position &left, const Position &right) {
    return left.x == right.x && left.y == right.y;
}

// Function that gets Agent object from position data
Agent &getAgent(std::vector<Agent> &population, Position pos) {
    for (int i = 0; i < population.size(); i++) {
        if (pos == population[i].pos) {
            return population[i];
        }
    }
}

// Some sort of search function that takes world array and creates a vector contains the adjacent tiles
std::vector<int> getAdjacent(int rowsize, Agent &agent) {
    // map[y*rowsize + x]
    std::vector<int> adjacent;
    for (int i = 0; i < agent.radius * 2 + 1; i++) {
        for (int j = 0; j < agent.radius * 2 + 1; j++) {
            int x = agent.pos.x + i - agent.radius;
            int y = agent.pos.x + j - agent.radius;
            int index = (y * rowsize ) + x;
            adjacent.push_back(map[index]);
        }
    }
    return adjacent;
}

void control(Agent &agent, int rowsize) {
    // Get adjacent tiles
    std::vector<int> adjacent = getAdjacent(rowsize, agent);
    std::vector<float> weightMap;

    // convert adjacent tiles to weights
    for (int i = 0; i < adjacent.size(); i++) {
        switch (adjacent[i]) {
            case 0:
                // Empty space
                weightMap.push_back(agent.moveWeights[0]);
                break;
            case 1:
                // Agent
                weightMap.push_back(agent.moveWeights[1]);
                break;
            case 2:
                // Wall
                weightMap.push_back(agent.moveWeights[2]);
                break;

            default:
                // Energy
                int energyValue = adjacent[i] - 155;
                weightMap.push_back((adjacent[i] - 155) * agent.moveWeights[3]);
                break;
        }
    }

    // Calculate directional weights
    // Up
    float upWeight;
    for (int i = 0; i < agent.radius * 2 + 1; i++) {
        for (int j = 0; j < agent.radius; j++) {
            int x = i - agent.radius;
            int y = j - agent.radius;
            int index = (y * (agent.radius * 2 + 1)) + x;
            upWeight += weightMap[index];
        }
    }
    // Down
    float downWeight;
    for (int i = 0; i < agent.radius * 2 + 1; i++) {
        for (int j = agent.radius + 1; j < agent.radius * 2 + 1; j++) {
            int x = i - agent.radius;
            int y = j - agent.radius;
            int index = (y * (agent.radius * 2 + 1)) + x;
            downWeight += weightMap[index];
        }
    }
    // Left
    float leftWeight;
    for (int i = 0; i < agent.radius; i++) {
        for (int j = 0; j < agent.radius * 2 + 1; j++) {
            int x = i - agent.radius;
            int y = j - agent.radius;
            int index = (y * (agent.radius * 2 + 1)) + x;
            leftWeight += weightMap[index];
        }
    }

    // Right
    float rightWeight;
    for (int i = agent.radius + 1; i < agent.radius * 2 + 1; i++) {
        for (int j = 0; j < agent.radius * 2 + 1; j++) {
            int x = i - agent.radius;
            int y = j - agent.radius;
            int index = (y * (agent.radius * 2 + 1)) + x;
            rightWeight += weightMap[index];
        }
    }

    // Choose direction
    // normalize weights
    float weightSum = upWeight + downWeight + leftWeight + rightWeight;
    upWeight = upWeight/weightSum;
    downWeight = downWeight/weightSum;
    leftWeight = leftWeight/weightSum;
    rightWeight = rightWeight/weightSum;

    float r = nRand();

    // Check what is in that tile
    // if wall
    // do not move
    // if agent
    // do not move and steal it's energy
    // if energy
    // Add energy to energy storage
    // delete energy from map
    // move agent into that space
    // consume some energy
    // remove previous energy position
}

// Some sort of pathing finding algorithm that takes target position and generate paths avoiding objects that are not the target
// This is greatly simplified if it can only see the squares on it sides.

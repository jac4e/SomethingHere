#include "../world/map.h"
#include "ai.h"

extern Map map;

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
            int index = (y * rowsize) + x;
            adjacent.push_back(map[index]);
        }
    }
    return adjacent;
}

void control(Agent &agent, std::vector<Agent> &population, int rowsize) {
    // check if energy is

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
    std::vector<float> directionWeights = {upWeight / weightSum, downWeight / weightSum, leftWeight / weightSum, rightWeight / weightSum};

    float r = nRand();
    int index;

    for (int i = 0; i < directionWeights.size(); i++) {
        // printf("size: %d", population.size());
        if (i == directionWeights.size() - 1 || (r > directionWeights[i] && r < directionWeights[i + 1])) {
            index = i;
        }
    }

    // Check what is in that tile
    Position targetPos;
    int targetValue;

    switch (index) {
        case 0:
            // Up
            targetPos = {agent.pos.x, agent.pos.y + 1};
            int targetIndex = (targetPos.y * rowsize) + targetPos.x;
            targetValue = map[targetIndex];
            break;
        case 1:
            // Down
            targetPos = {agent.pos.x, agent.pos.y - 1};
            int targetIndex = (targetPos.y * rowsize) + targetPos.x;
            targetValue = map[targetIndex];
            break;
        case 2:
            // Left
            targetPos = {agent.pos.x - 1, agent.pos.y};
            int targetIndex = (targetPos.y * rowsize) + targetPos.x;
            targetValue = map[targetIndex];
            break;
        case 3:
            // Right
            targetPos = {agent.pos.x + 1, agent.pos.y};
            int targetIndex = (targetPos.y * rowsize) + targetPos.x;
            targetValue = map[targetIndex];
            break;
        default:
            targetValue = 0;
            targetPos = {0, 0};
            break;
    }

    switch (targetValue) {
        case 0:
            // Empty
            int oldIndex = (agent.pos.y * rowsize) + agent.pos.x;
            int newIndex = (targetPos.y * rowsize) + targetPos.x;

            map[oldIndex] = 0;
            map[newIndex] = 1;

            break;
        case 1:
            // Agent
            // do not move and steal it's energy
            Agent targetAgent = getAgent(population, targetPos);
            targetAgent.stealEnergy(agent.str * 10);
            break;
        case 2:
            // Wall
            // do not move
            break;

        default:
            // energy
            int oldIndex = (agent.pos.y * rowsize) + agent.pos.x;
            int newIndex = (targetPos.y * rowsize) + targetPos.x;
            int energy = (map[newIndex] - 155);
            agent.energyStorage += energy;
            map[oldIndex] = 0;
            map[newIndex] = 1;
            break;
    }
}
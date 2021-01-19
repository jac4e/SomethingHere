#include <stdio.h>

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

void move(Agent &agent, Position target) {
    // This should not be hard coded
    if (agent.useEnergy(10)) {
        map.setCell(agent.pos, 0);
        map.setCell(target, 1);

        agent.pos = target;
        }
    }

void control(Agent &agent, std::vector<Agent> &population) {
    if (agent.deathTime > 0) {
        return;
    } else if (agent.energyStorage <= 0) {
        agent.kill();
        return;
}

    // should loop through all agents in here or make population global

    // Get adjacent tiles
    std::vector<unsigned char> adjacent = map.getCellRadius(agent.pos, agent.radius);
    std::vector<float> weightMap;

    // convert adjacent tiles to weight map
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
                int energyValue = adjacent[i] - 154;
                weightMap.push_back(energyValue * agent.moveWeights[3]);
                break;
        }
    }

    // Calculate directional weights
    std::vector<float> directionWeights(4);
    // Up
    float upWeight = 0;
    for (int i = 0; i < agent.radius * 2 + 1; i++) {
        for (int j = 0; j < agent.radius; j++) {
            upWeight += weightMap[j * (agent.radius * 2) + i];
        }
    }
    directionWeights[0] = upWeight;
    // Down
    float downWeight = 0;
    for (int i = 0; i < agent.radius * 2 + 1; i++) {
        for (int j = agent.radius + 1; j < agent.radius * 2 + 1; j++) {
            downWeight += weightMap[(j * (agent.radius * 2 + 1)) + i];
        }
    }
    directionWeights[1] = downWeight;
    // Left
    float leftWeight = 0;
    for (int i = 0; i < agent.radius; i++) {
        for (int j = 0; j < agent.radius * 2 + 1; j++) {
            leftWeight += weightMap[(j * (agent.radius * 2 + 1)) + i];
        }
    }
    directionWeights[2] = leftWeight;
    // Right
    float rightWeight = 0;
    for (int i = agent.radius + 1; i < agent.radius * 2 + 1; i++) {
        for (int j = 0; j < agent.radius * 2 + 1; j++) {
            rightWeight += weightMap[(j * (agent.radius * 2 + 1)) + i];
        }
    }
    directionWeights[3] = rightWeight;

    // Choose direction
    // normalize weights
    float weightSum = upWeight + downWeight + leftWeight + rightWeight;
    float probSum = 0;
    for (int i = 0; i < directionWeights.size(); i++) {
        probSum += directionWeights[i] / weightSum;
        directionWeights[i] = probSum;
    }

    float r = nRand();
    int index = rand() % 4;

    for (int i = 0; i < directionWeights.size(); i++) {
        // printf("size: %d", population.size());
        if (r < directionWeights[i]) {
            index = i;
            break;
        }
    }

    // Check what is in that tile
    Position targetPos;
    int targetValue = 0;

    if (index == 0) {
            // Up
            targetPos = {agent.pos.x, agent.pos.y + 1};
    } else if (index == 1) {
            // Down
            targetPos = {agent.pos.x, agent.pos.y - 1};
    } else if (index == 2) {
            // Left
            targetPos = {agent.pos.x - 1, agent.pos.y};
    } else if (index == 3) {
            // Right
            targetPos = {agent.pos.x + 1, agent.pos.y};
    } else {
            targetValue = 0;
            targetPos = {0, 0};
    }
    targetValue = map.getCell(targetPos);

    if (targetValue == 0) {
            // Empty
        move(agent, targetPos);
    } else if (targetValue == 1) {
            // Agent
            // do not move and steal it's energy
        Agent &targetAgent = getAgent(population, targetPos);
        if (agent.useEnergy(10)) {
            agent.energyStorage += targetAgent.stealEnergy(agent.str * 10);
        }

        // These should not be hard coded
    } else if (targetValue == 2) {
            // Wall
        // do not move but consume slight amount of energy
        agent.useEnergy(5);
    } else {
            // energy
        if (agent.useEnergy(10)) {
            agent.energyStorage += (targetValue - 154);
            move(agent, targetPos);
        }
    }
}
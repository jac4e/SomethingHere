#include <stdio.h>

#include <iostream>
#include <random>
#include <vector>

#include "ai/agent.h"
#include "ai/ai.h"
#include "renderingEngine/draw_screen.h"
#include "world/map.h"

// #include "simulation/tick_code.h"

std::random_device rd;
std::mt19937 mersenne(rd());

extern std::vector<int> deathList;

Map map;
std::vector<Agent> population;
struct display_data dat;

int time = 0;
int deaths = 0;

// Generation Statistics initialization
float highest = 0;
int genHighest = 0;
int generation = 0;

void init() {
    // Generate map
    int dim = 1;
    int amt = 1;
    printf("Enter map dimension\n");
    scanf_s("%d", &dim);
    printf("Enter agent amt. ");
    do {
        printf("Must be multiple of 4: ");
        scanf_s("%d", &amt);
        printf("\n");
    } while (amt % 4);
    map.generate(dim, 100, 0, 500, 1, 20);

    // Generate agents and place in map
    population = generateAgents(amt, 2);
    map.placeAgents(population);

    // Pass reference of map to display data then initialize display data and screen
    dat.types = map.map;
    int dims[2] = {map.width, map.height};
    init_dsp_data(&dat, dims);
    init_screen(&dat);
}

// Update runs each tick
void update() {
    // Update screen
    update_screen(&dat);

    // Update positions of each agent
    for (int i = 0; i < population.size(); i++) {
        control(population[i], population, i);
    }
}

// Generation reset
void reset() {
    deathList.clear();
    float maxLit = 0;
    float sum = 0;
    float avg = 0;
    for (int i = 0; i < population.size(); i++) {
        calculateLitness(population[i]);
        sum += population[i].litness;
        if (population[i].litness > maxLit) {
            maxLit = population[i].litness;
            genHighest = i;
        }
    }
    avg = sum / population.size();
    if (maxLit > highest) {
        highest = maxLit;
    }
    printf("Generation %d - genHighest %.0f - 5 gen average %.0f - Overall %f.0\n", generation, maxLit, avg, highest);
    deaths = 0;
    time = 0;
    reproducePopulation(population);
    ++generation;
    genHighest = 0;
    map.randomize();
    map.placeAgents(population);
}

int main() {
    init();

    // Per generation loop
    while (1) {
        // Per tick loop
        while (1) {
            update();
            if (deaths >= population.size()) {
                break;
            }
            ++time;
        }
        reset();
    }
    return 0;
}

#include <stdio.h>

#include <iostream>
#include <vector>
#include <random>

#include "ai/ai.h"
#include "renderingEngine/draw_screen.h"
#include "ai/agent.h"
#include "world/map.h"

// #include "renderingEngine/draw_screen.h"
// #include "renderingEngine/textures.h"
// #include "simulation/tick_code.h"

std::random_device rd;
std::mt19937 mersenne(rd());

extern std::vector<int> deathList;

Map map;

int time = 0;
int deaths = 0;
int main() {
    int dim;
    printf("Enter map dimension\n");
    std::cin >> dim;
    
    int amt;
    printf("Enter agent amt\n");
    std::cin >> amt;
    map.generate(dim, 100, 200, 1000, 1, 20);
    struct display_data dat;
    dat.types = map.map;
	int dims[2] = {map.width, map.height};
	init_dsp_data(&dat, dims);
    init_screen(&dat);


    float highest = 0;
    int genHighest = 0;
    int generation = 0;
    std::vector<Agent> population = generateAgents(amt, 10);
    //printf("Starting...\n");
    while (1) {
        deaths = 0;
        time = 0;
        //printf("Placing agents...\n");
        map.placeAgents(population);
        //map.print();
        //std::cin.ignore();
        while (1) {
            //wait for user input to move to next frame
            update_screen(&dat);
            //std::cin.ignore();
            for (int i = 0; i < population.size(); i++) {
                if (population[i].energyStorage <= 0) {
                    //std::cout << "dead agent\n";
                }
                //printMap();
                //map.print();
                control(population[i], population, i);
            }
            if (deaths >= population.size()) {
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
                //printf("Gen highest weights:\n\tEmpty: %f\n\tAgent: %f\n\tWall: %f\n\tEnergy: %f\n", population[genHighest].moveWeights[0], population[genHighest].moveWeights[1], population[genHighest].moveWeights[2], population[genHighest].moveWeights[3]);

                if (maxLit > highest)
                {
                    highest = maxLit;
                }
                printf("Generation %d - genHighest %.0f - 5 gen average %.0f - Overall %f.0\n", generation, maxLit, avg, highest);
                break;
            }
            ++time;
        }
        //printf("Reproducing agents...\n");
        reproducePopulation(population);
        ++generation;
        genHighest = 0;
        map.randomize();
    }
    return 0;
}

#include <stdio.h>

#include <iostream>
#include <vector>
#include <random>

#include "ai/ai.h"
#include "ai/agent.h"
#include "world/map.h"

// #include "renderingEngine/draw_screen.h"
// #include "renderingEngine/textures.h"
// #include "simulation/tick_code.h"

std::random_device rd;
std::mt19937 mersenne(rd());

Map map(100, 100, 0, 1000, 0, 10);

int time = 0;
int deaths = 0;
int main() {
    int highest = 0;
    int genHighest = 0;
    int generation = 0;
    std::vector<Agent> population = generateAgents(20, 100);
    printf("Starting...\n");
    while (1) {
        deaths = 0;
        time = 0;
        printf("Placing agents...\n");
        map.placeAgents(population);
        while (1) {
            //wait for user input to move to next frame
            std::cin.ignore();
            map.print();
            for (int i = 0; i < population.size(); i++) {
                if (population[i].energyStorage <= 0) {
                    //std::cout << "dead agent\n";
                }
                //printMap();
                control(population[i], population);
            }
            if (deaths == population.size()) {
                for (int i = 0; i < population.size(); i++) {
                    calculateLitness(population[i]);
                    
                    if (population[i].energyUsed > genHighest) {
                        genHighest = population[i].litness;
                    }
                }
                if (genHighest > highest)
                {
                    highest = genHighest;
                }
                printf("Generation %d\n - genHighest %d - Overall %d\n", generation, genHighest, highest);
                break;
            }
            ++time;
        }
        printf("Reproducing agents...\n");
        reproducePopulation(population);
        ++generation;
        genHighest = 0;
        map.clear();
    }

    // struct display_data dat;
    // struct tick_vars tick;

    // // testing
    // int dims[2] = {50, 20};

    // init_dsp_data(&dat, dims);

    // init_screen(&dat);
    // setup_clock(&tick);

    // while (1) {
    //     // compute new cycle time delta
    //     tick_delta(&tick);

    //     while (tick.delta > 0) {
    //         // do per-tick things
    //         --tick.delta;
    //         ++tick.ticks;
    //     }

    //     // update screen as frequently as possible
    //     update_screen(&dat);
    //     print_fps(&tick);
    //     ++tick.frames;
    // }

    return 0;
}

#include "ai.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Agent::Agent() {
};

void Agent::setPos(int x, int y) {
    pos = {x,y};
    return;
};

void Agent::setProperties(float a, float b, int c, int d, int e){
    agg = a;
    picky = b;
    stge = c;
    str = d;
    agi = e;
}

std::vector<Agent> generateAgents(int amt, int skillMax){
    std::vector<Agent> agentList(amt);

    srand(time(NULL));

    for (int i = 0; i < amt; i++)
    {
        // Generate floats between 0 and 1 by calculating the normal of rand()
        float agg = (float) rand()/RAND_MAX;
        float picky = (float) rand()/RAND_MAX;
        
        // Generate three numbers that sum to skillMax by randomly generating two cut-off points

        int a = 1;
        int b = 1;

        while ( a==b || a>b || b>=skillMax)
        {
            a = rand() % skillMax + 1;
            b = rand() % skillMax + 1;
        }

        int stge = a;
        int str = b-a;
        int agi = skillMax-b;

        // Set the properties based on the randomly generated values
        agentList[i].setProperties(agg, picky, stge, str, agi);
    }
    return agentList;
};
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


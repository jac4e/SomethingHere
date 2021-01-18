#include <stdio.h>

#include <iostream>
#include <stdexcept>
#include <random>

#include "../ai/ai.h"
#include "position.h"
#include "map.h"

extern std::mt19937 mersenne;

Map::Map(int w, int h, int wDen, int eDen, int eMin, int eMax) {
    width = w;
    height = h;
    wallDensity = wDen;
    energyDensity = eDen;
    energyMin = eMin;
    energyMax = eMax;

    map = new unsigned char[height * width]{};

    randomize();
}

unsigned char *&Map::getMap(){
    return map;
}

unsigned char Map::getCell(Position position) {
    if (position.x >= width || position.y >= height ||
        position.x < 0 || position.y < 0) {
        // return wall if position out of bounds
        return 2;
    }
    return map[position.y * width + position.x];
}

void Map::setCell(Position position, unsigned char type) {
    // Check that position is inside map range
    if (position.x >= width || position.y >= height ||
        position.x < 0 || position.y < 0) {
        throw std::out_of_range("position out of range");
    }
    // set value
    map[position.y * width + position.x] = type;
}

void Map::clear() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            map[y * width, x] = 0;
        }
    }
}

void Map::randomize() {
    // Generate walls
    // shapes: | L + [] () .
    /*
		weight, out of 256 and representation:
		0 = .  - 52
		1 = |  - 76
		2 = L  - 42
		3 = +  - 42
		4 = [] - 22
		5 = () - 22
		and the weight placements are:
		0 '.' 51, 52 '|' 127 'L' 169 '+' 211 '[]' 233 '()' 255
		(note: each range has = amounts of odd and even numbers)
	*/

    // 2 = wall
    printf("Generating map..\n");
    auto placeWall = [&](int y, int x) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            bool noNearbyWall = true;
            for (int i = -1; i <= 1; ++i) {
                if (x + i < 0)
                    ++i;
                else if (x + i == width)
                    break;
                for (int j = -1; j <= 1; ++j) {
                    if (y + j < 0)
                        ++j;
                    else if (y + j == height)
                        break;
                    if (map[(y + j) * width + (x + i)] == 2) {
                        noNearbyWall = false;
                        goto exit;
                    }
                }
            }
        exit:
            if (noNearbyWall)
                map[y * width + x] = 1;  // placeholder so that shapes can be built without this function denying them the right to exist
        }
    };

    auto commitWall = [&](int xMin, int xL, int yMin, int yL) {
        for (int i = 0; i < xL; ++i)
            for (int j = 0; j < yL; ++j)
                if (xMin + i >= 0 &&
                    xMin + i < width &&
                    yMin + j >= 0 &&
                    yMin + j < height &&
                    map[(yMin + j) * width + (xMin + i)] == 1)
                    map[(yMin + j) * width + (xMin + i)] = 2;
    };

    printf("Placing walls..\n");
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (mersenne() % 10000 < wallDensity) {
                unsigned char currentShape = mersenne();
                if (currentShape <= 51)  //single wall
                {
                    placeWall(y, x);
                    commitWall(x, 1, y, 1);
                    //cout << ".\n";
                } else if (currentShape <= 211)  // line
                {
                    placeWall(y, x);
                    bool horizontal = currentShape % 2 == 0;
                    unsigned char length = 76 % 4;
                    if (length == 0)
                        length = 1;
                    if (horizontal) {
                        for (int i = 1; i <= length; ++i) {
                            placeWall(y, x + i);
                            placeWall(y, x - i);
                        }
                    } else {
                        for (int i = 1; i <= length; ++i) {
                            placeWall(y + i, x);
                            placeWall(y - i, x);
                        }
                    }
                    horizontal = !horizontal;  //because the second piece, if applicable, is the opposite orientation
                    if (currentShape > 169)    // +
                    {
                        unsigned char topL = (mersenne() % (length + 1)) + 1;
                        unsigned char botL = (mersenne() % (length + 1)) + 1;
                        int position = mersenne() % (length * 2 + 1) - length;
                        if (horizontal) {
                            for (int i = 1; i <= topL; ++i)
                                placeWall(y + position, x + i);
                            for (int i = 1; i <= botL; ++i)
                                placeWall(y + position, x - i);

                        } else {
                            for (int i = 1; i <= topL; ++i)
                                placeWall(y + i, x + position);
                            for (int i = 1; i <= botL; ++i)
                                placeWall(y - i, x + position);
                        }
                        //cout << "+\n";
                    } else if (currentShape > 127)  // L
                    {
                        bool top = mersenne() % 2 == 0;
                        bool right = mersenne() % 2 == 0;
                        unsigned char newL = (length * 2) + mersenne() % 3 - 1;
                        if (horizontal) {
                            for (int i = 1; i <= newL; ++i) {
                                if (right)
                                    placeWall(top ? y + length : y - length, x + i);
                                else
                                    placeWall(top ? y + length : y - length, x - i);
                            }
                        } else {
                            for (int i = 1; i <= newL; ++i) {
                                if (right)
                                    placeWall(y + i, top ? x + length : x - length);
                                else
                                    placeWall(y - i, top ? x + length : x - length);
                            }
                        }
                        //cout << "L\n";
                    }
                    //cout << "|\n";
                } else if (currentShape <= 233)  // []
                {
                    unsigned char length = mersenne() % 5 + 1;
                    unsigned char height = mersenne() % 5 + 1;
                    unsigned char startX = x - (length / 2);
                    unsigned char startY = y - (height / 2);
                    for (int iy = 0; iy < height; ++iy)
                        for (int ix = 0; ix < length; ++ix)
                            placeWall(startY + iy, startX + ix);
                    //cout << "[]\n";
                } else if (currentShape <= 255)  // <>
                {
                    unsigned char radius = mersenne() % 3 + 1;
                    unsigned char startX = x - radius;
                    unsigned char startY = y - radius;
                    for (int iy = 0; iy <= radius * 2; ++iy)
                        for (int ix = abs(radius - iy); ix <= radius * 2 - abs(radius - iy); ++ix)
                            placeWall(startY + iy, startX + ix);
                    //cout << "<>\n";
                }
                commitWall(0, width, 0, height);
                //print(map);
                //cout << "\n\n\n\n";
            }
        }
    }
    printf("Placing energy..\n");
    for (int i = 0; i < width * height; ++i)
        if (mersenne() % 10000 < energyDensity && map[i] != 2) {
            map[i] = 154 + mersenne() % (energyMax - energyMin + 1) + energyMin;
        }
}

void Map::placeAgents(std::vector<Agent> &population) {
    int x;
    int y;
    for (int i = 0; i < population.size(); ++i) {
        do {
            x = mersenne() % width;
            y = mersenne() % height;
        } while (map[y * width + x] != 0);
        population[i].setPosition(x, y);
        map[y * width + x] = 1;
    }
}

std::vector<unsigned char> Map::getCellRadius(Position position, int radius) {
    std::vector<unsigned char> adjacent;
    for (int i = 0; i < radius * 2 + 1; i++) {
        for (int j = 0; j < radius * 2 + 1; j++) {
            Position pos = { position.x + j - radius , position.y + i - radius };
            adjacent.push_back(getCell(pos));
        }
    }
    return adjacent;
}
void Map::print() {
    std::cout << "+-";
    for (int i = 0; i < width; ++i)
        std::cout << "---";
    std::cout << "+\n";
    for (int y = 0; y < height; ++y) {
        std::cout << "| ";
        for (int x = 0; x < width; ++x) {
            Position pos = { x, y };
            if (getCell(pos) == 0) {
                // Empty
                std::cout << " _ ";
            }
            else if (getCell(pos) == 1) {
                // Agent 
                std::cout << " A ";
            }
            else if (getCell(pos) == 2) {
                // Wall
                std::cout << " W ";
            }
            else {
                // energy
                std::cout << " E "; 
                // map.getCell(pos) - 154;
            }
        }
        std::cout << "|\n";
    }
    std::cout << "+-";
    for (int i = 0; i < width; ++i)
        std::cout << "---";
    std::cout << "+\n";
}

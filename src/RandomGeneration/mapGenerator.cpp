#include <stdio.h>
#include <iostream>
#include <random>
#include "../ai/ai.hpp"
using std::cout;

// density out of 10'000 in both following cases
int dWall;
int dEnergy;
int xSize;
int ySize;
int minEnergy; // <= 0
int maxEnergy; // >= 10
extern unsigned char *map;
std::random_device rd;
std::mt19937 mersenne(rd());

void print()
{
	// in the map, 0 = nothing, 1 = agent, 2 = wall, 155-255 = energy cells containing 1-100 energy
	for (int i = 0; i < xSize; ++i)
		cout << "--";
	cout << "+\n";
	for (int y = 0; y < ySize; ++y)
	{
		for (int x = 0; x < xSize; ++x)
		{
			if (map[y * xSize + x] == 0)
				cout << "  ";
			else if (map[y * xSize + x] == 1)
				cout << " A";
			else if (map[y * xSize + x] == 2)
				cout << " W";
			else
				cout << 'E' << map[y*xSize + x] - 154;
		}
		cout << "|\n";
	}
	for (int i = 0; i < xSize; ++i)
		cout << "--";
	cout << "+\n";
}

void placeWall(int y, int x)
{
	if (x >= 0 && x < xSize && y >= 0 && y < ySize)
	{
		bool noNearbyWall = true;
		for (int i = -1; i <= 1; ++i)
		{
			if (x + i < 0)
				++i;
			else if (x + i == xSize)
				break;
			for (int j = -1; j <= 1; ++j)
			{
				if (y + j < 0)
					++j;
				else if (y + j == ySize)
					break;
				if (map[(y + j) * xSize + (x + i)] == 2)
				{
					noNearbyWall = false;
					goto exit;
				}
			}
		}
	exit:
		if (noNearbyWall)
			map[y * xSize + x] = 1; // placeholder so that shapes can be built without this function denying them the right to exist
	}
}

void commitWall(int xMin, int xL, int yMin, int yL)
{
	for (int i = 0; i < xL; ++i)
		for (int j = 0; j < yL; ++j)
			if (xMin + i >= 0 && xMin + i < xSize && yMin + j >= 0 && yMin + j < ySize && map[(yMin + j) * xSize + (xMin + i)] == 1)
				map[(yMin + j) * xSize + (xMin + i)] = 2;
}

void generateWalls()
{
	//shapes: | L + [] () .
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

	for (int y = 0; y < ySize; ++y)
	{
		for (int x = 0; x < xSize; ++x)
		{
			if (mersenne() % 10000 < dWall)
			{
				unsigned char currentShape = mersenne();
				if (currentShape <= 51) //single wall
				{
					placeWall(y, x);
					commitWall(x, 1, y, 1);
					//cout << ".\n";
				}
				else if (currentShape <= 211) // line 
				{
					placeWall( y, x);
					bool horizontal = currentShape % 2 == 0;
					unsigned char length = 76 % 4;
					if (length == 0)
						length = 1;
					if (horizontal)
					{
						for (int i = 1; i <= length; ++i)
						{
							placeWall( y, x + i);
							placeWall( y, x - i);
						}
					}
					else
					{
						for (int i = 1; i <= length; ++i)
						{
							placeWall( y + i, x);
							placeWall( y - i, x);
						}
					}
					horizontal = !horizontal; //because the second piece, if applicable, is the opposite orientation
					if (currentShape > 169) // +
					{
						unsigned char topL = (mersenne() % (length + 1)) + 1;
						unsigned char botL = (mersenne() % (length + 1)) + 1;
						int position = mersenne() % (length * 2 + 1) - length;
						if (horizontal)
						{
							for (int i = 1; i <= topL; ++i)
								placeWall( y + position, x + i);
							for (int i = 1; i <= botL; ++i)
								placeWall( y + position, x - i);

						}
						else
						{
							for (int i = 1; i <= topL; ++i)
								placeWall( y + i, x + position);
							for (int i = 1; i <= botL; ++i)
								placeWall( y - i, x + position);

						}
						//cout << "+\n";
					}
					else if (currentShape > 127) // L
					{
						bool top = mersenne() % 2 == 0;
						bool right = mersenne() % 2 == 0;
						unsigned char newL = (length * 2) + mersenne() % 3 - 1;
						if (horizontal)
						{
							for (int i = 1; i <= newL; ++i)
							{
								if (right)
									placeWall( top ? y + length : y - length, x + i);
								else
									placeWall( top ? y + length : y - length, x - i);
							}
						}
						else
						{
							for (int i = 1; i <= newL; ++i)
							{
								if (right)
									placeWall( y + i, top ? x + length : x - length);
								else
									placeWall( y - i, top ? x + length : x - length);
							}
						}
						//cout << "L\n";
					}
						//cout << "|\n";
				}
				else if (currentShape <= 233) // []
				{
					unsigned char length = mersenne() % 5 + 1;
					unsigned char height = mersenne() % 5 + 1;
					unsigned char startX = x - (length / 2);
					unsigned char startY = y - (height / 2);
					for (int iy = 0; iy < height; ++iy)
						for (int ix = 0; ix < length; ++ix)
							placeWall( startY + iy, startX + ix);
					//cout << "[]\n";
				}
				else if (currentShape <= 255) // <>
				{
					unsigned char radius = mersenne() % 3 + 1;
					unsigned char startX = x - radius;
					unsigned char startY = y - radius;
					for (int iy = 0; iy <= radius * 2; ++iy)
						for (int ix = abs(radius - iy); ix <= radius * 2 - abs(radius - iy); ++ix)
								placeWall( startY + iy, startX + ix);
					//cout << "<>\n";
				}
				commitWall( 0, xSize, 0, ySize);
				//print(map);
				//cout << "\n\n\n\n";
			}
		}
	}
}

void generateEcells()
{
	for (int i = 0; i < xSize * ySize; ++i)
		if (mersenne() % 10000 < dEnergy && map[i] != 2)
		{
			map[i] = 154 + mersenne() % (maxEnergy - minEnergy + 1) + minEnergy;
		}
}

void placeActors(std::vector<Agent> agent)
{
	int x;
	int y;
	for (int i = 0; i < agent.size(); ++i)
	{
		do {
			x = mersenne() % xSize;
			y = mersenne() % ySize;
		} while (map[y*xSize + x] != 0);
		agent[i].setPosition(x, y);
		map[y*xSize + x] = 1;
	}
}

void generateMap(int wallDensity, int energyDensity, int sizeX, int sizeY, int energyMin, int energyMax, std::vector<Agent> agents)
{
	// mersenne() = random number

	/*
		1) randomize walls
		2) place energy, randomize it
		3) place actors
	*/

	// in the map, 0 = nothing, 1 = agent, 2 = wall, 156-255 = energy cells containing 1-100 energy

	dWall = wallDensity;
	dEnergy = energyDensity;
	xSize = sizeX;
	ySize = sizeY;
	minEnergy = energyMin; // <= 0
	maxEnergy = energyMax; // >= 10

	map = new unsigned char[ySize*xSize];

	generateWalls();
	generateEcells();
	placeActors(agents);

	//print(map);
}

/*
int main()
{
	while (true)
	{
		generateMap(100, 1000, 0, 80, 50, 1, 9);
		system("pause");
	}

	return 0;
}
*/

#pragma once
// Creating various position based stuff useful for 2D worlds

struct Position {
    int x;
    int y;
};

bool operator==(const Position &left, const Position &right);
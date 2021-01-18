#include "position.h"

bool operator==(const Position &left, const Position &right) {
    return left.x == right.x && left.y == right.y;
}
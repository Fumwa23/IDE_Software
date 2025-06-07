#include "SaveManager.h"

void SaveManager::save(int position, int coords[2]) {
    if (position < 1 || position > NUM_POINTS) return;
    savePoints[position - 1] = {coords[0], coords[1]};
}

Coordinate SaveManager::fetch(int position) {
    if (position < 1 || position > NUM_POINTS) return {0, 0};
    return savePoints[position - 1];
}

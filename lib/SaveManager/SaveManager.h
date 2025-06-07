#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

struct Coordinate {
    int x;
    int y;
};

class SaveManager {
public:
    void save(int position, int coords[2]);
    Coordinate fetch(int position);

private:
    static const int NUM_POINTS = 5;
    Coordinate savePoints[NUM_POINTS];
};

#endif

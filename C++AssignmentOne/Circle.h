#pragma once
#include "Shape.h"
#include "Movable.h"

using namespace std;

class Circle : public Shape, public Movable {
public:
    Circle (int x, int y, int radius);
    virtual void calculateArea();
    virtual void calculatePerimeter();
    virtual void calculatePoints();
    virtual string toString();
    virtual void Scale(float x, float y);
    int radius;
};
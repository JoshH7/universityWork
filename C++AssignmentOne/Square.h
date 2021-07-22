#pragma once
#include "Shape.h"
#include "Movable.h"

using namespace std;

class Square : public Shape, public Movable  {
public:
    Square(int x, int y, int edge);
    virtual void calculateArea();
    virtual void calculatePerimeter();
    virtual void calculatePoints();
    virtual string toString();
    virtual void Scale(float x, float y);
    int edge;
};
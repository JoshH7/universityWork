#pragma once
#include "Shape.h"
#include "Movable.h"

using namespace std;

class Rectangle : public Shape, public Movable  {
public:
    Rectangle(int x, int y, int w, int h);
    virtual void calculateArea();
    virtual void calculatePerimeter();
    virtual void calculatePoints();
    virtual string toString();
    virtual void Scale(float x, float y);
    int width;
    int height;
};
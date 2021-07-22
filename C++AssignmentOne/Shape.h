#pragma once
#include "Point.h"
#include <vector>
#include <string>

using namespace std;

class Shape{
    public:
    int perimeter;
    Point * leftTop;
    vector<Point*> Points;
    int area;
    bool isCircular = false;
    virtual void calculateArea() = 0;
    virtual void calculatePerimeter() = 0;
    virtual void calculatePoints() = 0;
    virtual string toString() = 0;
};

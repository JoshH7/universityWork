#pragma once

using namespace std;

class Movable{
public:
    virtual void Move(int x, int y);
    virtual void Scale(float x, float y) = 0;
};
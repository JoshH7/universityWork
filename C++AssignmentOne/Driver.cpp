
/* ------------------------------------------------------
CMP2801M: Advanced Programming
Driver program for assignment
Fall 2019

Written by Ayse Kucukyilmaz

This file is a representative test file.
During marking, we will use the exact same notation
as provided in the brief, so make sure
you follow that guideline. Also make sure that you don't
change the main body provided to you here.
Otherwise, your code may not pass the test cases...

GOOD LUCK!

------------------------------------------------------ */
#include "Point.h"
#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Square.h"
#include "Movable.h"

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <math.h>

using namespace std;

string point_vector_to_string (vector<Point*> &v) {
    string output = "Points [";

    for(std::vector<Point*>::iterator it = v.begin(); it != v.end(); ++it) {
        output += "(" + to_string((*it)->x) + "," + to_string((*it)->y) + ")";
    }

    output += "]";

    return output;
}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

Rectangle::Rectangle(int x, int y, int w, int h) {
    this->leftTop = new Point(x, y);
    this->width=w;
    this->height=h;
    this->Points.push_back(this->leftTop);
    this->Points.push_back(new Point(this->leftTop->x + this->width, this->leftTop->y)); // rightTop
    this->Points.push_back(new Point(this->leftTop->x, this->leftTop->y + this->height)); // leftBottom
    this->Points.push_back(new Point(this->leftTop->x + this->width, this->leftTop->y + this->height)); // rightBottom
    this->calculateArea();
    this->calculatePerimeter();
}

void Rectangle::calculateArea() {
    this->area = this->width * this->height;
}

void Rectangle::calculatePerimeter() {
    this->perimeter = this->width + this->width + this->height + this->height;
}

void Rectangle::calculatePoints() {
    this->Points[1]->x = this->leftTop->x + this->width; // rightTop
    this->Points[1]->y = this->leftTop->y;
    this->Points[2]->x = this->leftTop->x; // leftBottom
    this->Points[2]->y = this->leftTop->y + this->height;
    this->Points[3]->x = this->leftTop->x + this->width; // rightBottom
    this->Points[3]->y = this->leftTop->y + this->height;
}

string Rectangle::toString () {
    return "Rectangle[h=" + to_string(this->height) + ",w=" + to_string(this->height) + "]\n" + point_vector_to_string(this->Points) + " Area=" + to_string(this->area) + " Perimeter=" + to_string(this->perimeter);
}


Square::Square(int x, int y, int edge) {
    this->edge = edge;
    this->leftTop = new Point(x, y);
    this->Points.push_back(this->leftTop);
    this->Points.push_back(new Point(this->leftTop->x + this->edge, this->leftTop->y)); // rightTop
    this->Points.push_back(new Point(this->leftTop->x, this->leftTop->y + this->edge)); // leftBottom
    this->Points.push_back(this->Points[3] = new Point(this->leftTop->x + this->edge, this->leftTop->y + this->edge)); // rightBottom
    this->calculateArea();
    this->calculatePerimeter();
}

void Square::calculateArea() {
    this->area = this->edge * this->edge;
}

void Square::calculatePerimeter() {
    this->perimeter = this->edge * 4;
}

void Square::calculatePoints() {
    this->Points[1]->x = this->leftTop->x + this->edge; // rightTop
    this->Points[1]->y = this->leftTop->y;
    this->Points[2]->x = this->leftTop->x; // leftBottom
    this->Points[2]->y = this->leftTop->y + this->edge;
    this->Points[3]->x = this->leftTop->x + this->edge; // rightBottom
    this->Points[3]->y = this->leftTop->y + this->edge;
}

string Square::toString () {
    return "Square[e=" + to_string(this->edge) + "]\n" + point_vector_to_string(this->Points) + " Area=" + to_string(this->area) + " Perimeter=" + to_string(this->perimeter);
}



Circle::Circle(int x, int y, int radius) {
    this->radius = radius;
    this->leftTop = new Point(x, y);
    this->Points.push_back(this->leftTop);
    this->Points.push_back(new Point(x + (radius * 2), y + (radius * 2))); // bottom right
    this->calculateArea();
    this->calculatePerimeter();
}

void Circle::calculateArea() {
    this->area = 3.14159 * pow(this->radius, 2);
}

void Circle::calculatePerimeter() {
    this->perimeter = 2 * 3.14159 * this->radius;
}

void Circle::calculatePoints() {
    this->Points[1]->x = this->leftTop->x + (this->radius * 2); // rightBottom
    this->Points[1]->y = this->leftTop->y + (this->radius * 2);
}

string Circle::toString () {
    return "Circle[r=" + to_string(this->radius) + "]\n" + point_vector_to_string(this->Points) + " Area=" + to_string(this->area) + " Perimeter=" + to_string(this->perimeter);
}

vector<string> split_string_into_vector(const string &str, const char seps) {
    vector<string> splatted = vector<string>();
    const int length = str.size();
    int lastSplit = 0;
    int i = 0;
    while (i < length) {
        char check = str[i];

        if (check == seps) {
            const string part = str.substr(lastSplit, i - lastSplit);
            splatted.push_back(part);
            lastSplit = i + 1;
        }

        i++;
    }

    string lastPart = str.substr(lastSplit, length - lastSplit);
    splatted.push_back(lastPart);

    return splatted;
}

void Circle::Scale(float x, float y) {
    if (x != y) throw "Shape must be scaled by same X and Y.";
    this->radius *= x;
    this->calculatePoints();
    this->calculateArea();
    this->calculatePerimeter();
}

void Square::Scale(float x, float y) {
    if (x != y) throw "Shape must be scaled by same X and Y.";
    this->edge *= x;
    this->calculatePoints();
    this->calculateArea();
    this->calculatePerimeter();
}

void Rectangle::Scale(float x, float y) {
    this->width *= x;
    this->height *= y;
    this->calculatePoints();
    this->calculateArea();
    this->calculatePerimeter();
}

void Movable::Move(int x, int y) {
    Shape* shape = dynamic_cast<Shape*>(this);
    if (!shape) throw "Cannot move the movable as it's not a shape.";
    shape->leftTop->x += x;
    shape->leftTop->y += y;
    shape->calculatePoints();
}

int main()
{
    string userCommand;
    vector <Shape*> shapes;     // this one will hold your shapes

    while (userCommand.compare("exit") != 0)
    {
//        try {
            cout << "Enter the command: ";

            getline(cin, userCommand);

            vector<string> parameters = split_string_into_vector(userCommand,
                                                                 ' '); // this one will hold parameters for the commands


            const string command = parameters[0];

//            cout << command << "\n";

            // in the following code, consider checking for the arguments.
            // in case of too few arguments, you may remind the user the correct format

            if (command.compare("addR") == 0) {
                if (parameters.size() != 5) throw exception(); // throw error if bad number of arguments

                const int x = stoi(parameters[1]);
                const int y = stoi(parameters[2]);
                const int w = stoi(parameters[3]);
                const int h = stoi(parameters[4]);

                Rectangle *rect = new Rectangle(x, y, w, h);
                shapes.push_back(rect);
                cout << rect->toString() + "\n";
            } else if (command.compare("addS") == 0) {
                if (parameters.size() != 4) throw exception(); // throw error if bad number of arguments

                const int x = stoi(parameters[1]);
                const int y = stoi(parameters[2]);
                const int e = stoi(parameters[3]);

                Square *square = new Square(x, y, e);
                shapes.push_back(square);
                cout << square->toString() + "\n";
            }
            if (command.compare("addC") == 0) {
                if (parameters.size() != 4) throw exception(); // throw error if bad number of arguments

                const int x = stoi(parameters[1]);
                const int y = stoi(parameters[2]);
                const int r = stoi(parameters[3]);

                Circle *circle = new Circle(x, y, r);
                shapes.push_back(circle);
                cout << circle->toString() + "\n";
            } else if (command.compare("scale") == 0) {
                if (parameters.size() != 4) throw exception(); // throw error if bad number of arguments

                // scale object at index... the scaling needs to be isotropic in case of circle and square
                // you may want to check if the index exists or not!

                const int index = stoi(parameters[1]) - 1; // minus 1 because indexes start at 1 according to spec
                const float x = stof(parameters[2]);
                const float y = stof(parameters[3]);

                if (index < 0 || index >= shapes.size()) throw "No shape exists at this index"; // throw exception if out of bounds

                Shape* shape = shapes[index];

                // Multiple inhertitance is tricky! The Shape class does nto have a scale function, the Movable does!
                // As a result all your derived classes have scale functions...
                // You may need to use type casting wisely to use polymorphic functionality!
                Movable* m = dynamic_cast<Movable*>(shape); // cast down
                if (!m) throw "Could not cast the shape to a movable object.";

                // scale and print
                m->Scale(x, y);

                cout << shape->toString();

            } else if (command.compare("move") == 0) {
                if (parameters.size() != 4) throw exception(); // throw error if bad number of arguments
                const int index = stoi(parameters[1]) - 1; // minus 1 because indexes start at 1 according to spec
                const int x = stoi(parameters[2]);
                const int y = stoi(parameters[3]);

                if (index < 0 || index >= shapes.size()) throw "No shape exists at this index"; // throw exception if out of bounds

                Shape* shape = shapes[index];

                Movable* m = dynamic_cast<Movable*>(shape); // cast down
                if (!m) throw "Could not cast the shape to a movable object.";

                m->Move(x, y);

                cout << shape->toString();
            } else if (command.compare("display") == 0) {
                // this is not given in our example, but why don't you implement a display function which shows all objects stored in shapes?
            }

            // do any necessary postprocessing at the end of each loop...
            // yes, there is some necessary postprocessing...
            cout << endl << endl;
//        } catch (exception) {
//            cout << exception;
//        }
    }

    cout << "Press any key to continue...";
    std::getchar();

    return 0;
}

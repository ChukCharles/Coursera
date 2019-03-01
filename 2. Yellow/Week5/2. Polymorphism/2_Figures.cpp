#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <math.h>
#include <memory>
#include <vector>

using namespace std;

class Figure {
public:

    Figure(const string& n) : name(n) {
    }

    virtual string Name() const {
        return name;
    }
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
private:
    const string name;
};

//Triangle, Rect и Circle
//CreateFigure

class Triangle : public Figure {
public:

    Triangle(int s1, int s2, int s3)
    : Figure("TRIANGLE"), side1(s1), side2(s2), side3(s3) {
    }

    double Perimeter() const override {
        double res = side1 + side2 + side3;
        return res;
    }

    double Area() const override {
        double p = Perimeter() / 2;
        double a = p * (p - side1)*(p - side2)*(p - side3);
        return sqrt(a);
    }

private:
    const int side1, side2, side3;
};

class Rect : public Figure {
public:

    Rect(int a, int b) : Figure("RECT"), side1(a), side2(b) {
    }

    double Perimeter() const override {
        return 2 * (side1 + side2);
    }

    double Area() const override {
        return side1*side2;
    }
private:
    const int side1, side2;
};

class Circle : public Figure {
public:

    Circle(int r) : Figure("CIRCLE"), radius(r) {
    }

    double Perimeter() const override {
        return 2 * 3.14 * radius;
    }

    double Area() const override {
        return 3.14 * pow(radius, 2);
    }

private:
    const int radius;
};

shared_ptr<Figure> CreateFigure(istringstream& is) {
    string type;
    is >> type;

    if (type == "RECT") {
        int x = 0, y = 0;
        is >> x >> y;
        return make_shared<Rect>(x, y);
    } else if (type == "CIRCLE") {
        int r = 0;
        is >> r;
        return make_shared<Circle>(r);
    } else if (type == "TRIANGLE") {
        int x = 0, y = 0, z = 0;
        is >> x >> y >> z;
        return make_shared<Triangle>(x, y, z);
    } else {
        cout << "ERROR: unknown figure type\n";
        return 0;
    }
}

int main() {
    /*Triangle t1( 3, 2, 1 );
    cout << t1.Perimeter() << " " << t1.Area() << endl;
    Rect r1( 2, 5 );
    cout << r1.Name() << " " << r1.Perimeter() << " " << r1.Area() << endl;
    Circle c1( 1 );
    cout << c1.Name() << " " << c1.Perimeter() << " " << c1.Area() << endl;
    
    shared_ptr<Figure> f1 = CreateFigure();
    cout << f1->Name() << " " << f1->Perimeter() << " " << f1->Area() << endl;*/

    vector<shared_ptr < Figure>> figures;
    for (string line; getline(cin, line);) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                        << current_figure->Name() << " "
                        << current_figure->Perimeter() << " "
                        << current_figure->Area() << endl;
            }
        }
    }

    return 0;
}
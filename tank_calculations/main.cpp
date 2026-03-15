#include <iostream>
#include <vector>

using namespace std;

const float PI = 3.141592653589793;

float Volume (float diameter, float height) {
    float r = diameter / 2;
    return PI * r * r * height;
}

struct CylinderTank {
    int capacity;
    float diameter;
    float height;
    float volume;

    CylinderTank(int c, float d, float h) {
        capacity = c;
        diameter = d;
        height = h;
        volume = Volume(d, h);
    }

    void printCylinderTankDetails() const {
        cout << "Cylinder Tank Details:" << endl;
        cout << " Capacity: " << capacity << " gallons" << endl;
        cout << " Diameter: " << diameter << " ft" << endl;
        cout << " Height: " << height << " ft" << endl;
        cout << " Volume: " << volume << " cubic ft" << endl;
        cout << "-------------------------" << endl;
    }
};

int main()
{
    // t_2k
    float t_2kCap = 2000;
    vector<float> t_2kDem = {8, 5.3};
    CylinderTank t_2k{t_2kCap, t_2kDem[0], t_2kDem[1]};
    t_2k.printCylinderTankDetails();

    // t_5k
    float t_5kCap = 5000;
    vector<float> t_5kDem = {10, 8.5};
    CylinderTank t_5k{t_5kCap, t_5kDem[0], t_5kDem[1]};
    t_5k.printCylinderTankDetails();

    // t_10k
    float t_10kCap = 10000;
    vector<float> t_10kDem = {12, 11.8};
    CylinderTank t_10k{t_10kCap, t_10kDem[0], t_10kDem[1]};
    t_10k.printCylinderTankDetails();

    // t_15k
    float t_15kCap = 15000;
    vector<float> t_15kDem = {14, 13.0};
    CylinderTank t_15k{t_15kCap, t_15kDem[0], t_15kDem[1]};
    t_15k.printCylinderTankDetails();

    // t_20k
    float t_20kCap = 20000;
    vector<float> t_20kDem = {16, 13.3};
    CylinderTank t_20k{t_20kCap, t_20kDem[0], t_20kDem[1]};
    t_20k.printCylinderTankDetails();

    return 0;
}
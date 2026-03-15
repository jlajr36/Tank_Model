#include <iostream>

using namespace std;

const float PI = 3.141592653589793;

float Volume (float diameter, float height) {
    float r = diameter / 2;
    return PI * r * r * height;
}

struct Tank {
    int capacity;
    float diameter;
    float height;
    float volume;

    Tank(int c, float d, float h) {
        capacity = c;
        diameter = d;
        height = h;
        volume = Volume(d, h);
    }

    void print() const {
        cout << "Tank Details:" << endl;
        cout << " Capacity: " << capacity << " gallons" << endl;
        cout << " Diameter: " << diameter << " ft" << endl;
        cout << " Height: " << height << " ft" << endl;
        cout << " Volume: " << volume << " cubic ft" << endl;
        cout << "-------------------------" << endl;
    }
};

int main()
{
    cout << "Volumes" << endl;

    Tank t_2k{2000,   8,  5.3};
    Tank t_5k{5000,   10, 8.5};
    Tank t_10k{10000, 12, 11.8};
    Tank t_15k{15000, 14, 13.0};
    Tank t_20k{20000, 16, 13.3};

    t_2k.print();
    t_5k.print();
    t_10k.print();
    t_15k.print();
    t_20k.print();

    return 0;
}

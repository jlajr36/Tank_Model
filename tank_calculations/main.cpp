#include <iostream>
#include <vector>

using namespace std;

const float PI = 3.141592653589793;

struct Pipe {
    int diameter_in;      // Pipe diameter in inches
    float velocity_ft_s; // Flow velocity in ft/s

    // Calculate area in ft²
    float area() const {
        float D_ft = diameter_in / 12.0;
        return PI * D_ft * D_ft / 4.0;
    }

    // Calculate flow in GPM
    float flow() const {
        return area() * velocity_ft_s * 7.4805 * 60.0;
    }

    // Print pipe info
    void print() const {
        cout << "Pipe " << diameter_in << "\" : Area = "
             << area() << " ft squared, Flow = "
             << flow() << " GPM" << endl;
    }
};

struct CylinderTank {
    int capacity;       // Capacity of the tank in gallons
    float diameter;     // Diameter of the cylinder in feet
    float height;       // Height of the cylinder in feet
    float volume;       // Volume of the cylinder in cubic feet

    // Constructor: initializes the tank with capacity, diameter, and height
    // Also calculates the volume upon creation
    CylinderTank(int c, float d, float h) 
        : capacity(c), diameter(d), height(h) 
    {
        volume = Volume(); // Calculate initial volume
    }

    // Function to calculate the cylinder's volume in cubic feet
    float Volume() const {
        float r = diameter / 2.0f;  // Radius = diameter / 2
        return PI * r * r * height; // Volume formula: π * r^2 * h
    }

    // Function to print all details of the cylinder tank
    void printCylinderTankDetails() const {
        cout << "Cylinder Tank Details:" << endl;
        cout << " Capacity: " << capacity << " gallons" << endl;
        cout << " Diameter: " << diameter << " ft" << endl;
        cout << " Height: " << height << " ft" << endl;
        cout << " Volume: " << volume << " cubic ft" << endl;
        cout << " Volume (approx gallons): " << volume * 7.48052f << " gallons" << endl; // Convert cubic feet to gallons
        cout << "-------------------------" << endl;
    }
};

int main()
{
    float velocity_ft_s = 5.0; // 5 ft/s

    // 1-inch pipe
    Pipe p_1in;
    p_1in.diameter_in = 1;          
    p_1in.velocity_ft_s = velocity_ft_s;
    p_1in.print();

    // 2-inch pipe
    Pipe p_2in;
    p_2in.diameter_in = 2;          
    p_2in.velocity_ft_s = velocity_ft_s;
    p_2in.print();

    // 3-inch pipe
    Pipe p_3in;
    p_3in.diameter_in = 3;
    p_3in.velocity_ft_s = velocity_ft_s;
    p_3in.print();

    // 4-inch pipe
    Pipe p_4in;
    p_4in.diameter_in = 4;
    p_4in.velocity_ft_s = velocity_ft_s;
    p_4in.print();

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
#include <iostream>
#include <vector>
#include <iomanip> // For fixed and setprecision

using namespace std;

const float PI = 3.141592653589793f;

struct Pipe {
    int diameter_in;      // Pipe diameter in inches
    float velocity_ft_s;  // Flow velocity in ft/s

    // Calculate area in ft²
    float area() const {
        float D_ft = diameter_in / 12.0f; // convert inches to feet
        return PI * D_ft * D_ft / 4.0f;
    }

    // Calculate flow in GPM
    float flow() const {
        return area() * velocity_ft_s * 7.48052f * 60.0f;
    }

    // Print pipe info
    void print() const {
        cout << fixed << setprecision(3);
        cout << "Pipe " << diameter_in << "\" : Area = "
             << area() << " ft^2, Flow = "
             << flow() << " GPM" << endl;
    }
};

struct CylinderTank {
    int capacity;       // Capacity in gallons
    float diameter;     // Diameter in feet
    float height;       // Height in feet

    // Constructor
    CylinderTank(int c, float d, float h) 
        : capacity(c), diameter(d), height(h) {}

    // Calculate cylinder volume in cubic feet
    float volume() const {
        float r = diameter / 2.0f;
        return PI * r * r * height;
    }

    // Print tank details
    void printCylinderTankDetails() const {
        cout << fixed << setprecision(2);
        cout << "Cylinder Tank Details:" << endl;
        cout << " Capacity: " << capacity << " gallons" << endl;
        cout << " Diameter: " << diameter << " ft" << endl;
        cout << " Height: " << height << " ft" << endl;
        cout << " Volume: " << volume() << " cubic ft" << endl;
        cout << " Volume (approx gallons): " << volume() * 7.48052f << " gallons" << endl;
        cout << "-------------------------" << endl;
    }
};

int main()
{
    float velocity_ft_s = 5.0f; // 5 ft/s

    // Pipes
    Pipe p_1in{1, velocity_ft_s};
    p_1in.print();

    Pipe p_2in{2, velocity_ft_s};
    p_2in.print();

    Pipe p_3in{3, velocity_ft_s};
    p_3in.print();

    Pipe p_4in{4, velocity_ft_s};
    p_4in.print();

    // Tanks
    CylinderTank t_2k{2000, 8.0f, 5.3f};
    t_2k.printCylinderTankDetails();

    CylinderTank t_5k{5000, 10.0f, 8.5f};
    t_5k.printCylinderTankDetails();

    CylinderTank t_10k{10000, 12.0f, 11.8f};
    t_10k.printCylinderTankDetails();

    CylinderTank t_15k{15000, 14.0f, 13.0f};
    t_15k.printCylinderTankDetails();

    CylinderTank t_20k{20000, 16.0f, 13.3f};
    t_20k.printCylinderTankDetails();

    return 0;
}
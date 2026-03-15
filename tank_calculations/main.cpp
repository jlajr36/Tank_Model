#include <iostream>
#include <vector>
#include <iomanip> // For fixed and setprecision

using namespace std;

const float PI = 3.141592653589793f;

struct Pipe {
    int diameter_in;      // Pipe diameter in inches
    float velocity_ft_s;  // Flow velocity in ft/s

    float area() const {
        float D_ft = diameter_in / 12.0f;
        return PI * D_ft * D_ft / 4.0f;
    }

    float flow() const {
        return area() * velocity_ft_s * 7.48052f * 60.0f;
    }

    void print() const {
        cout << fixed << setprecision(3);
        cout << "Pipe " << diameter_in << "\" : Area = "
             << area() << " ft^2, Flow = "
             << flow() << " GPM" << endl;
    }
};

struct CylinderTank {
    int capacity;
    float diameter;
    float height;

    CylinderTank(int c, float d, float h) 
        : capacity(c), diameter(d), height(h) {}

    float volume() const {
        float r = diameter / 2.0f;
        return PI * r * r * height;
    }

    float fillTimeMinutes(const Pipe& pipe) const {
        float flow_gpm = pipe.flow();
        if (flow_gpm <= 0.0f) return -1;
        return capacity / flow_gpm;
    }

    float drainTimeMinutes(const Pipe& pipe) const {
        float flow_gpm = pipe.flow();
        if (flow_gpm <= 0.0f) return -1;
        return capacity / flow_gpm;
    }

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

    void printFillTime(const Pipe& pipe) const {
        float t = fillTimeMinutes(pipe);
        if (t < 0)
            cout << " Flow is zero or negative, cannot calculate fill time." << endl;
        else
            cout << " Fill time with " << pipe.diameter_in << "\" pipe: "
                 << fixed << setprecision(2) << t << " minutes" << endl;
    }

    void printDrainTime(const Pipe& pipe) const {
        float t = drainTimeMinutes(pipe);
        if (t < 0)
            cout << " Flow is zero or negative, cannot calculate drain time." << endl;
        else
            cout << " Drain time with " << pipe.diameter_in << "\" pipe: "
                 << fixed << setprecision(2) << t << " minutes" << endl;
    }
};

int main()
{
    float velocity_ft_s = 5.0f;

    Pipe p_1in{1, velocity_ft_s};
    p_1in.print();

    Pipe p_2in{2, velocity_ft_s};
    p_2in.print();

    Pipe p_3in{3, velocity_ft_s};
    p_3in.print();

    Pipe p_4in{4, velocity_ft_s};
    p_4in.print();

    CylinderTank t_2k{2000, 8.0f, 5.3f};
    cout << "=== Tank 2k ===" << endl;
    t_2k.printCylinderTankDetails();
    t_2k.printFillTime(p_1in);
    t_2k.printFillTime(p_2in);
    t_2k.printFillTime(p_3in);
    t_2k.printFillTime(p_4in);
    t_2k.printDrainTime(p_1in);
    t_2k.printDrainTime(p_2in);
    t_2k.printDrainTime(p_3in);
    t_2k.printDrainTime(p_4in);
    cout << endl;

    CylinderTank t_5k{5000, 10.0f, 8.5f};
    cout << "=== Tank 5k ===" << endl;
    t_5k.printCylinderTankDetails();
    t_5k.printFillTime(p_1in);
    t_5k.printFillTime(p_2in);
    t_5k.printFillTime(p_3in);
    t_5k.printFillTime(p_4in);
    t_5k.printDrainTime(p_1in);
    t_5k.printDrainTime(p_2in);
    t_5k.printDrainTime(p_3in);
    t_5k.printDrainTime(p_4in);
    cout << endl;

    CylinderTank t_10k{10000, 12.0f, 11.8f};
    cout << "=== Tank 10k ===" << endl;
    t_10k.printCylinderTankDetails();
    t_10k.printFillTime(p_1in);
    t_10k.printFillTime(p_2in);
    t_10k.printFillTime(p_3in);
    t_10k.printFillTime(p_4in);
    t_10k.printDrainTime(p_1in);
    t_10k.printDrainTime(p_2in);
    t_10k.printDrainTime(p_3in);
    t_10k.printDrainTime(p_4in);
    cout << endl;

    CylinderTank t_15k{15000, 14.0f, 13.0f};
    cout << "=== Tank 15k ===" << endl;
    t_15k.printCylinderTankDetails();
    t_15k.printFillTime(p_1in);
    t_15k.printFillTime(p_2in);
    t_15k.printFillTime(p_3in);
    t_15k.printFillTime(p_4in);
    t_15k.printDrainTime(p_1in);
    t_15k.printDrainTime(p_2in);
    t_15k.printDrainTime(p_3in);
    t_15k.printDrainTime(p_4in);
    cout << endl;

    CylinderTank t_20k{20000, 16.0f, 13.3f};
    cout << "=== Tank 20k ===" << endl;
    t_20k.printCylinderTankDetails();
    t_20k.printFillTime(p_1in);
    t_20k.printFillTime(p_2in);
    t_20k.printFillTime(p_3in);
    t_20k.printFillTime(p_4in);
    t_20k.printDrainTime(p_1in);
    t_20k.printDrainTime(p_2in);
    t_20k.printDrainTime(p_3in);
    t_20k.printDrainTime(p_4in);
    cout << endl;

    return 0;
}
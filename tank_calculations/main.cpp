#include <iostream>
#include <vector>
#include <iomanip> // For fixed and setprecision

using namespace std;

const float PI = 3.141592653589793f;

struct Pipe {
    int diameter_in;      
    float velocity_ft_s;  
    float resistance_factor; // 0 = no filter, 1 = fully blocked

    // Constructor
    Pipe(int dia, float vel, float resistance = 0.0f)
        : diameter_in(dia), velocity_ft_s(vel), resistance_factor(resistance) {}

    float area() const {
        float D_ft = diameter_in / 12.0f;
        return PI * D_ft * D_ft / 4.0f;
    }

    float flow() const {
        float raw_flow = area() * velocity_ft_s * 7.48052f * 60.0f;
        return raw_flow * (1.0f - resistance_factor); // Apply filter resistance
    }

    void print() const {
        cout << fixed << setprecision(3);
        cout << "Pipe " << diameter_in << "\" : Area = "
             << area() << " ft^2, Flow = "
             << flow() << " GPM";
        if (resistance_factor > 0.0f)
            cout << " (with " << resistance_factor*100 << "% filter resistance)";
        cout << endl;
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
        cout << fixed << setprecision(2);

        // Label pipe diameter and filter
        cout << " Drain out (" << pipe.diameter_in << "\"";
        if (pipe.resistance_factor > 0.0f)
            cout << ", with filter";   // show if filter is applied
        else
            cout << ", without filter"; // explicitly show no filter
        cout << "): ";

        // Show time or error
        if (t < 0)
            cout << "Flow is zero or negative, cannot calculate drain time." << endl;
        else
            cout << t << " minutes" << endl;
    }
};

int main()
{
    float velocity_ft_s = 5.0f;   // Flow velocity in feet per second (ft/s)
    float filter_res = 0.10f;     // 10% flow reduction due to filter

    // ------------------------
    // Pipes (no filter / with filter)
    // ------------------------
    Pipe p_1in{1, velocity_ft_s};                  
    Pipe p_1out{1, velocity_ft_s, filter_res};    

    Pipe p_2in{2, velocity_ft_s};                 
    Pipe p_2out{2, velocity_ft_s, filter_res};    

    Pipe p_3in{3, velocity_ft_s};                 
    Pipe p_3out{3, velocity_ft_s, filter_res};    

    Pipe p_4in{4, velocity_ft_s};                 
    Pipe p_4out{4, velocity_ft_s, filter_res};    

    // ------------------------
    // Tank 2k
    // ------------------------
    CylinderTank t_2k{2000, 8.0f, 5.3f};
    cout << "=== Tank 2k ===" << endl;
    t_2k.printCylinderTankDetails();

    t_2k.printFillTime(p_1in);
    t_2k.printFillTime(p_2in);
    t_2k.printFillTime(p_3in);
    t_2k.printFillTime(p_4in);

    t_2k.printDrainTime(p_1out);
    t_2k.printDrainTime(p_2out);
    t_2k.printDrainTime(p_3out);
    t_2k.printDrainTime(p_4out);
    cout << endl;

    // ------------------------
    // Tank 5k
    // ------------------------
    CylinderTank t_5k{5000, 10.0f, 8.5f};
    cout << "=== Tank 5k ===" << endl;
    t_5k.printCylinderTankDetails();

    t_5k.printFillTime(p_1in);
    t_5k.printFillTime(p_2in);
    t_5k.printFillTime(p_3in);
    t_5k.printFillTime(p_4in);

    t_5k.printDrainTime(p_1out);
    t_5k.printDrainTime(p_2out);
    t_5k.printDrainTime(p_3out);
    t_5k.printDrainTime(p_4out);
    cout << endl;

    // ------------------------
    // Tank 10k
    // ------------------------
    CylinderTank t_10k{10000, 12.0f, 11.8f};
    cout << "=== Tank 10k ===" << endl;
    t_10k.printCylinderTankDetails();

    t_10k.printFillTime(p_1in);
    t_10k.printFillTime(p_2in);
    t_10k.printFillTime(p_3in);
    t_10k.printFillTime(p_4in);

    t_10k.printDrainTime(p_1out);
    t_10k.printDrainTime(p_2out);
    t_10k.printDrainTime(p_3out);
    t_10k.printDrainTime(p_4out);
    cout << endl;

    // ------------------------
    // Tank 15k
    // ------------------------
    CylinderTank t_15k{15000, 14.0f, 13.0f};
    cout << "=== Tank 15k ===" << endl;
    t_15k.printCylinderTankDetails();

    t_15k.printFillTime(p_1in);
    t_15k.printFillTime(p_2in);
    t_15k.printFillTime(p_3in);
    t_15k.printFillTime(p_4in);

    t_15k.printDrainTime(p_1out);
    t_15k.printDrainTime(p_2out);
    t_15k.printDrainTime(p_3out);
    t_15k.printDrainTime(p_4out);
    cout << endl;

    // ------------------------
    // Tank 20k
    // ------------------------
    CylinderTank t_20k{20000, 16.0f, 13.3f};
    cout << "=== Tank 20k ===" << endl;
    t_20k.printCylinderTankDetails();

    t_20k.printFillTime(p_1in);
    t_20k.printFillTime(p_2in);
    t_20k.printFillTime(p_3in);
    t_20k.printFillTime(p_4in);

    t_20k.printDrainTime(p_1out);
    t_20k.printDrainTime(p_2out);
    t_20k.printDrainTime(p_3out);
    t_20k.printDrainTime(p_4out);
    cout << endl;

    return 0;
}
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

const float PI = 3.141592653589793f;

// ------------------------
// Pipe Struct
// ------------------------
struct Pipe {
    int diameter_in;
    float velocity_ft_s;
    float resistance_factor;

    Pipe(int dia, float vel, float resistance = 0.0f)
        : diameter_in(dia), velocity_ft_s(vel), resistance_factor(resistance) {}

    float area() const {
        float D_ft = diameter_in / 12.0f;
        return PI * D_ft * D_ft / 4.0f;
    }

    float flow() const {
        float raw_flow = area() * velocity_ft_s * 7.48052f * 60.0f;
        return raw_flow * (1.0f - resistance_factor);
    }
};

// ------------------------
// Level Sensor Struct
// ------------------------
struct LevelSensor {
    string name;
    float trigger_level; // 0.0–1.0
    float reaction_time; // minutes

    LevelSensor(string n, float level, float rt)
        : name(n), trigger_level(level), reaction_time(rt) {}
};

// ------------------------
// CylinderTank Struct
// ------------------------
struct CylinderTank {
    int capacity;
    float diameter;
    float height;
    vector<LevelSensor> sensors;

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

    float fillTimeToSensor(const Pipe& pipe, const LevelSensor& sensor) const {
        float flow_gpm = pipe.flow();
        if (flow_gpm <= 0.0f) return -1;
        float target_volume = capacity * sensor.trigger_level;
        return (target_volume / flow_gpm) + sensor.reaction_time;
    }

    float drainTimeToSensor(const Pipe& pipe, const LevelSensor& sensor) const {
        float flow_gpm = pipe.flow();
        if (flow_gpm <= 0.0f) return -1;
        float target_volume = capacity * sensor.trigger_level;
        return (target_volume / flow_gpm) + sensor.reaction_time;
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

        cout << " Drain out (" << pipe.diameter_in << "\"";
        if (pipe.resistance_factor > 0.0f)
            cout << ", with filter";  
        else
            cout << ", without filter"; 
        cout << "): ";

        if (t < 0)
            cout << "Flow is zero or negative, cannot calculate drain time." << endl;
        else
            cout << t << " minutes" << endl;
    }

    void printFillToSensor(const Pipe& pipe, const LevelSensor& sensor) const {
        float t = fillTimeToSensor(pipe, sensor);
        cout << fixed << setprecision(2);
        cout << " Fill to " << sensor.name << " sensor (" 
             << sensor.trigger_level*100 << "%): ";
        if (t < 0) cout << "Flow is zero or negative." << endl;
        else cout << t << " minutes (including sensor reaction time)" << endl;
    }

    void printDrainToSensor(const Pipe& pipe, const LevelSensor& sensor) const {
        float t = drainTimeToSensor(pipe, sensor);
        cout << fixed << setprecision(2);
        cout << " Drain to " << sensor.name << " sensor (" 
             << sensor.trigger_level*100 << "%): ";
        if (t < 0) cout << "Flow is zero or negative." << endl;
        else cout << t << " minutes (including sensor reaction time)" << endl;
    }
};

// ------------------------
// Pump Struct with Flow Boost
// ------------------------
struct Pump {
    string name;
    float max_gpm;
    float flow_boost_factor;

    Pump(string n, float gpm, float boost = 1.0f)
        : name(n), max_gpm(gpm), flow_boost_factor(boost) {}
};

// ------------------------
// Pump Recommendation Function
// ------------------------
Pump recommendPump(const CylinderTank& tank, const Pipe& pipe, 
                   const vector<Pump>& pumps, float desired_fill_time_min) {
    float required_flow = tank.capacity / desired_fill_time_min;

    for (const auto& pump : pumps) {
        float boosted_flow = pipe.flow() * pump.flow_boost_factor;
        float effective_flow = min(boosted_flow, pump.max_gpm);

        if (effective_flow >= required_flow) {
            return pump;
        }
    }

    return Pump("No suitable pump", 0.0f);
}

// ------------------------
// Main Function (Fully Expanded, No Arrays)
// ------------------------
int main()
{
    float velocity_ft_s = 5.0f;
    float filter_res = 0.10f;

    // ------------------------
    // Pipes
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
    // Pumps
    // ------------------------
    vector<Pump> availablePumps = {
        Pump("Pump Small", 50.0f, 2.0f),
        Pump("Pump Medium", 200.0f, 2.5f),
        Pump("Pump Large", 500.0f, 3.0f),
        Pump("Pump X-Large", 1000.0f, 3.5f)
    };

    float desired_fill_time = 30.0f;

    // ------------------------
    // Level Sensors
    // ------------------------
    LevelSensor low("Low", 0.1f, 0.5f);
    LevelSensor mid("Middle", 0.5f, 0.2f);
    LevelSensor high("High", 0.9f, 0.3f);

    vector<LevelSensor> sensors = {low, mid, high};

    // ------------------------
    // Tank 2k
    // ------------------------
    CylinderTank t_2k{2000, 8.0f, 5.3f};
    t_2k.sensors = sensors;
    cout << "=== Tank 2k ===" << endl;
    t_2k.printCylinderTankDetails();

    t_2k.printFillTime(p_1in);
    cout << "  Recommended Pump: " << recommendPump(t_2k, p_1in, availablePumps, desired_fill_time).name << endl;

    t_2k.printFillTime(p_2in);
    cout << "  Recommended Pump: " << recommendPump(t_2k, p_2in, availablePumps, desired_fill_time).name << endl;

    t_2k.printFillTime(p_3in);
    cout << "  Recommended Pump: " << recommendPump(t_2k, p_3in, availablePumps, desired_fill_time).name << endl;

    t_2k.printFillTime(p_4in);
    cout << "  Recommended Pump: " << recommendPump(t_2k, p_4in, availablePumps, desired_fill_time).name << endl;

    t_2k.printDrainTime(p_1out);
    t_2k.printDrainTime(p_2out);
    t_2k.printDrainTime(p_3out);
    t_2k.printDrainTime(p_4out);

    // Fill/drain times to sensors
    for (auto& sensor : sensors) {
        t_2k.printFillToSensor(p_1in, sensor);
        t_2k.printFillToSensor(p_2in, sensor);
        t_2k.printFillToSensor(p_3in, sensor);
        t_2k.printFillToSensor(p_4in, sensor);

        t_2k.printDrainToSensor(p_1out, sensor);
        t_2k.printDrainToSensor(p_2out, sensor);
        t_2k.printDrainToSensor(p_3out, sensor);
        t_2k.printDrainToSensor(p_4out, sensor);
    }

    // ------------------------
    // Tank 5k
    // ------------------------
    CylinderTank t_5k{5000, 10.0f, 8.5f};
    t_5k.sensors = sensors;
    cout << "\n=== Tank 5k ===" << endl;
    t_5k.printCylinderTankDetails();

    t_5k.printFillTime(p_1in);
    cout << "  Recommended Pump: " << recommendPump(t_5k, p_1in, availablePumps, desired_fill_time).name << endl;

    t_5k.printFillTime(p_2in);
    cout << "  Recommended Pump: " << recommendPump(t_5k, p_2in, availablePumps, desired_fill_time).name << endl;

    t_5k.printFillTime(p_3in);
    cout << "  Recommended Pump: " << recommendPump(t_5k, p_3in, availablePumps, desired_fill_time).name << endl;

    t_5k.printFillTime(p_4in);
    cout << "  Recommended Pump: " << recommendPump(t_5k, p_4in, availablePumps, desired_fill_time).name << endl;

    t_5k.printDrainTime(p_1out);
    t_5k.printDrainTime(p_2out);
    t_5k.printDrainTime(p_3out);
    t_5k.printDrainTime(p_4out);

    for (auto& sensor : sensors) {
        t_5k.printFillToSensor(p_1in, sensor);
        t_5k.printFillToSensor(p_2in, sensor);
        t_5k.printFillToSensor(p_3in, sensor);
        t_5k.printFillToSensor(p_4in, sensor);

        t_5k.printDrainToSensor(p_1out, sensor);
        t_5k.printDrainToSensor(p_2out, sensor);
        t_5k.printDrainToSensor(p_3out, sensor);
        t_5k.printDrainToSensor(p_4out, sensor);
    }

    // ------------------------
    // Tank 10k
    // ------------------------
    CylinderTank t_10k{10000, 12.0f, 11.8f};
    t_10k.sensors = sensors;
    cout << "\n=== Tank 10k ===" << endl;
    t_10k.printCylinderTankDetails();

    t_10k.printFillTime(p_1in);
    cout << "  Recommended Pump: " << recommendPump(t_10k, p_1in, availablePumps, desired_fill_time).name << endl;

    t_10k.printFillTime(p_2in);
    cout << "  Recommended Pump: " << recommendPump(t_10k, p_2in, availablePumps, desired_fill_time).name << endl;

    t_10k.printFillTime(p_3in);
    cout << "  Recommended Pump: " << recommendPump(t_10k, p_3in, availablePumps, desired_fill_time).name << endl;

    t_10k.printFillTime(p_4in);
    cout << "  Recommended Pump: " << recommendPump(t_10k, p_4in, availablePumps, desired_fill_time).name << endl;

    t_10k.printDrainTime(p_1out);
    t_10k.printDrainTime(p_2out);
    t_10k.printDrainTime(p_3out);
    t_10k.printDrainTime(p_4out);

    for (auto& sensor : sensors) {
        t_10k.printFillToSensor(p_1in, sensor);
        t_10k.printFillToSensor(p_2in, sensor);
        t_10k.printFillToSensor(p_3in, sensor);
        t_10k.printFillToSensor(p_4in, sensor);

        t_10k.printDrainToSensor(p_1out, sensor);
        t_10k.printDrainToSensor(p_2out, sensor);
        t_10k.printDrainToSensor(p_3out, sensor);
        t_10k.printDrainToSensor(p_4out, sensor);
    }

    // ------------------------
    // Tank 15k
    // ------------------------
    CylinderTank t_15k{15000, 14.0f, 13.0f};
    t_15k.sensors = sensors;
    cout << "\n=== Tank 15k ===" << endl;
    t_15k.printCylinderTankDetails();

    t_15k.printFillTime(p_1in);
    cout << "  Recommended Pump: " << recommendPump(t_15k, p_1in, availablePumps, desired_fill_time).name << endl;

    t_15k.printFillTime(p_2in);
    cout << "  Recommended Pump: " << recommendPump(t_15k, p_2in, availablePumps, desired_fill_time).name << endl;

    t_15k.printFillTime(p_3in);
    cout << "  Recommended Pump: " << recommendPump(t_15k, p_3in, availablePumps, desired_fill_time).name << endl;

    t_15k.printFillTime(p_4in);
    cout << "  Recommended Pump: " << recommendPump(t_15k, p_4in, availablePumps, desired_fill_time).name << endl;

    t_15k.printDrainTime(p_1out);
    t_15k.printDrainTime(p_2out);
    t_15k.printDrainTime(p_3out);
    t_15k.printDrainTime(p_4out);

    for (auto& sensor : sensors) {
        t_15k.printFillToSensor(p_1in, sensor);
        t_15k.printFillToSensor(p_2in, sensor);
        t_15k.printFillToSensor(p_3in, sensor);
        t_15k.printFillToSensor(p_4in, sensor);

        t_15k.printDrainToSensor(p_1out, sensor);
        t_15k.printDrainToSensor(p_2out, sensor);
        t_15k.printDrainToSensor(p_3out, sensor);
        t_15k.printDrainToSensor(p_4out, sensor);
    }

    // ------------------------
    // Tank 20k
    // ------------------------
    CylinderTank t_20k{20000, 16.0f, 13.3f};
    t_20k.sensors = sensors;
    cout << "\n=== Tank 20k ===" << endl;
    t_20k.printCylinderTankDetails();

    t_20k.printFillTime(p_1in);
    cout << "  Recommended Pump: " << recommendPump(t_20k, p_1in, availablePumps, desired_fill_time).name << endl;

    t_20k.printFillTime(p_2in);
    cout << "  Recommended Pump: " << recommendPump(t_20k, p_2in, availablePumps, desired_fill_time).name << endl;

    t_20k.printFillTime(p_3in);
    cout << "  Recommended Pump: " << recommendPump(t_20k, p_3in, availablePumps, desired_fill_time).name << endl;

    t_20k.printFillTime(p_4in);
    cout << "  Recommended Pump: " << recommendPump(t_20k, p_4in, availablePumps, desired_fill_time).name << endl;

    t_20k.printDrainTime(p_1out);
    t_20k.printDrainTime(p_2out);
    t_20k.printDrainTime(p_3out);
    t_20k.printDrainTime(p_4out);

    for (auto& sensor : sensors) {
        t_20k.printFillToSensor(p_1in, sensor);
        t_20k.printFillToSensor(p_2in, sensor);
        t_20k.printFillToSensor(p_3in, sensor);
        t_20k.printFillToSensor(p_4in, sensor);

        t_20k.printDrainToSensor(p_1out, sensor);
        t_20k.printDrainToSensor(p_2out, sensor);
        t_20k.printDrainToSensor(p_3out, sensor);
        t_20k.printDrainToSensor(p_4out, sensor);
    }

    return 0;
}
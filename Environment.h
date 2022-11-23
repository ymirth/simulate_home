#pragma once
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<fstream>
#include<cmath>
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::cerr;
using std::unordered_map;
class Environment {
protected:
    double temperature_high;
    double temperature_low;
    double humidity;
    double air_speed;
public:
    Environment() {}

    virtual void load_data() = 0;
    const double& get_temperature_high()
    {
        return temperature_high;
    }
    const double& get_temperature_low()
    {
        return temperature_low;
    }
    const double& get_humidity()
    {
        return humidity;
    }
    const double& get_air_speed()
    {
        return air_speed;
    }
};
class Leaf_Model :public Environment {
private:
    string s_path;
    void load_data(string path) {
        fstream f;
        f.open(path);
        if (!f)
        {
            cerr << "Error: Data File of Environment couldn't be opened!" << endl;
            abort();
        }
        else
        {
            f >> humidity >> temperature_low >> temperature_high >> air_speed;
        }
    }
    virtual void load_data()
    {
        load_data(s_path);
    }
public:
    Leaf_Model() {}
    explicit Leaf_Model(string path) : s_path(path) {
        load_data();
    }

};

string base = "..//Home_Control_System//environment//";
string path_n[] = { base + "N_1.txt",base + "N_2.txt",base + "N_3.txt",base + "N_4.txt" };
string path_s[] = { base + "S_1.txt",base + "S_2.txt",base + "S_3.txt",base + "S_4.txt" };

class Env_Factory {
public:
    virtual Environment* get_season(string sea) = 0;
};
class North_Model :public Env_Factory {
private:
    static unordered_map<string, Environment*> season;
    static unordered_map<string, Environment*> initialize() {
        unordered_map<string, Environment*> model;
        //cout << path_n[0];
        model["spring"] = new Leaf_Model(path_n[0]);
        model["summer"] = new Leaf_Model(path_n[1]);
        model["fall"] = new Leaf_Model(path_n[2]);
        model["winter"] = new Leaf_Model(path_n[3]);
        return model;
    }
public:
    North_Model() {}
    virtual Environment* get_season(string sea)
    {
        return season[sea];
    }
};
unordered_map<string, Environment*> North_Model::season = North_Model::initialize();

class South_Model :public Env_Factory {
private:
    static unordered_map<string, Environment*> season;
    static unordered_map<string, Environment*> initialize() {
        unordered_map<string, Environment*> model;
        model["spring"] = new Leaf_Model(path_s[0]);
        model["summer"] = new Leaf_Model(path_s[1]);
        model["fall"] = new Leaf_Model(path_s[2]);
        model["winter"] = new Leaf_Model(path_s[3]);
        return model;
    }
public:
    South_Model() {};
    virtual Environment* get_season(string sea) {
        return season[sea];
    }
};
unordered_map<string, Environment*> South_Model::season = South_Model::initialize();

class Air
{
private:
    const static double air_density;						//g/L  or  kg/m^3
    const static double air_cp;								// kj/(kg*K)
    const static double Co2_Concentration_of_Environment;	//新风CO2浓度即大气里二氧化碳的浓度，约400-450ppm，我们按400算，即0.04%。


    Env_Factory* env;
    double humidity;                                        // relative_humidity under standard air preasure
    double temperature;
    double temp_high;
    double temp_low;
    double speed;
    double C;                                               // kj/(m^3*K)


public:
    Air()
    {
        int ns;
        cout << "Pleas select your area-- " << endl
            << "1: north " << "2: south :" << endl;
        cin >> ns;
        string season;
        cout << "Pleas input season: " << endl
            << "spring " << "summer " << " fall" << " winter : " << endl;
        cin >> season;
        if (ns == 1)
        {
            North_Model* n = new North_Model;
            set_environment(n, season);
        }
        else {
            South_Model* s = new South_Model;
            set_environment(s, season);
        }



        C = air_density * air_cp;
        display();
    }
    const double density() { return air_density; }
    const double cp() { return air_cp; }
    const double co2_concentration() { return Co2_Concentration_of_Environment; }
    const double air_temperature() { return temperature; }
    const double air_speed() { return speed; }
    const double air_C() { return C; }
    const double air_rh_humidity() { return humidity; }
    void set_environment(Env_Factory* en, string season) {
        env = en;
        Environment* temp = env->get_season(season);
        speed = temp->get_air_speed();
        humidity = temp->get_humidity();
        temp_high = temp->get_temperature_high();
        temp_low = temp->get_temperature_low();
        temperature = temp_low;
    }
    void display()
    {

        cout << "Air: " << endl
            << "highest_temperature = " << temp_high << "	"
            <<"lowest_temperature = " << temp_low << "	"
            <<"current_temperature= " << temperature << "	" << endl;
    }
    friend double simulate_temperature(int);

} air;
const double Air::air_density = 1.293;
const double Air::air_cp = 1.005;
const double Air::Co2_Concentration_of_Environment = 0.0005;

double simulate_temperature(int time)
{
    double b = pow(12 * 3600, 2) / (log(abs(air.temp_high) / abs(air.temp_low)));
    double result = air.temp_high * exp(-pow((time - 12 * 3600), 2) / b);
    air.temperature = result;
    return result;
}




#endif // !ENVIRONMENT_H

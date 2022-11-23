#pragma once
#ifndef DEVICE_H
#define DEVICE_H
#include<iostream>
#include<string>
#include<fstream>
using std::string;
using std::cout;
using std::endl;
using std::cerr;
class Device {  //??????
protected:
    int set = 0;					 //?????¦Ë 0<=set<mode
    int status;
    const int mode;
    double* energy_consumption;
public:
    explicit Device(const int& mode_) :mode(mode_), status(1)
    {
        energy_consumption = new double[mode];
    }
    Device(const int& mode_, const int& select_mode) :mode(mode_), status(1)
    {
        energy_consumption = new double[mode];
        Set_Mode(select_mode);
    }
    Device(const double* power_, const int& mode_) :mode(mode_), status(1)
    {
        energy_consumption = new double[mode];
        for (int i = 0; i < mode; ++i)
        {
            energy_consumption[i] = power_[i];
        }
    }
    Device(const double* power_, const int& mode_, const int& select_mode) :mode(mode_), status(1)
    {
        energy_consumption = new double[mode];
        for (int i = 0; i < mode; ++i)
        {
            energy_consumption[i] = power_[i];
        }
        Set_Mode(select_mode);
    }
    Device(const Device& copy) : mode(copy.mode), status(copy.status)
    {
        this->set = copy.set;
        energy_consumption = new double[mode];
        for (int i = 0; i < mode; ++i)
        {
            this->energy_consumption[i] = copy.energy_consumption[i];
        }
    }
    virtual ~Device()
    {
        delete[]energy_consumption;
    }


    //get the base's member
    virtual double get_set() { return set; }
    virtual double get_mode() { return mode; }
    virtual double get_status() { return status; }

    // Change member
    virtual void Set_Mode(const int& selected_mode)
    {
        if (selected_mode >= mode) {
            cerr << "The Selected Mode is out of range, please check the device's mode range";
            abort();
        }
        else {
            set = selected_mode;
        }
    };//???¨°?????
    virtual void Set_Statues(const int& state)
    {
        status = state;
    }
    // Search and Show
    virtual const string& get_version()const
    {
        return " ";
    }
    virtual void print_consumption()
    {
        cout << "The consumption is :" << energy_consumption[set] << endl;
    }
    virtual double get_consumption() { return status == 0 ? 0.0 : energy_consumption[set]; }

    virtual void print_info() = 0;  //???üD??

    // Virtual method get unique device parameter
    virtual const double New_Air() { return 0; }
    virtual const double get_heating_power()const { return 0; }
    virtual const double get_cooling_power()const { return 0; }
    virtual const double get_humid_amount()const { return 0; }
    virtual const double get_dehumid_amount()const { return 0; }

};
class AirConditioner :public Device
{
private:
    double Heating_power; //???????
    double Cooling_power;  //???Êé??
    double Circulating_air_volume;  //???????
    string version;
public:
    AirConditioner() :Device(2) {}  //????ûy????
    AirConditioner(const string& ver, const double* consume, const double& heat_power, const double& cool_power, const double& air_volume) :  //???????????
        Device(consume, 2), Circulating_air_volume(air_volume), version(ver), Heating_power(heat_power), Cooling_power(cool_power) {}
    AirConditioner(const AirConditioner& copy) :Device(copy), Heating_power(copy.Heating_power),
        Cooling_power(copy.Cooling_power), Circulating_air_volume(copy.Circulating_air_volume),
        version(copy.version) {}
    ~AirConditioner() {}

    virtual double get_set() { return Device::set; }
    virtual double get_mode() { return Device::mode; }
    virtual double get_status() { return Device::status; }


    virtual const string& get_version()const
    {
        return version;
    }
    const double get_Air_Volume() { return Circulating_air_volume; }
    virtual const double get_cooling_power()const { return  Device::set == 0 ? Cooling_power : 0; }
    virtual const double get_heating_power()const { return Device::set == 1 ? Heating_power : 0; }
    virtual double get_consumption() { return Device::status == 0 ? 0.0: energy_consumption[Device::set]; }
    virtual void print_info()
    {

        cout << "--------------------------------------------------------" << endl;
        cout << "This is a Air_Conditioner, and the version is: " << version << endl
            << "The Heating Power is: " << Heating_power << endl
            << "The Cooling Power is: " << Cooling_power << endl
            << "The Circulating Air Volume is: " << Circulating_air_volume << endl
            << "The Energy Consumption is: " << Device::energy_consumption[set] << endl;
        cout << "--------------------------------------------------------" << endl;
    }


};
//const int AirConditioner::kind = 1;
class Outdoor_Air :public Device
{
private:
    //const int mode;				 //??¦Ë
    double* Air_InAndOut_Rate;   //?????==??????
    double* energy_consumption;   //??????
    double* New_Air_Rate;
    string version;				 //???????
public:
    Outdoor_Air(const int mode_) :Device(mode_)
    {
        Air_InAndOut_Rate = new double[mode];
        energy_consumption = new double[mode];
        New_Air_Rate = new double[mode];
    }
    Outdoor_Air(const int mode_, double* IO, double* Energy, double* New_Air, const string version_) :Device(Energy, mode_), version(version_)
    {
        Air_InAndOut_Rate = new double[mode];
        New_Air_Rate = new double[mode];
        for (int i = 0; i < mode; ++i)
        {
            Air_InAndOut_Rate[i] = IO[i];
            New_Air_Rate[i] = New_Air[i];
        }
    }
    ~Outdoor_Air()
    {
        delete[]Air_InAndOut_Rate;
        delete[]New_Air_Rate;
        delete[]energy_consumption;
    }
    Outdoor_Air(const Outdoor_Air& copy) : Device(copy), version(copy.version)
    {
        Air_InAndOut_Rate = new double[mode];
        New_Air_Rate = new double[mode];
        for (int i = 0; i < mode; ++i)
        {
            Air_InAndOut_Rate[i] = copy.Air_InAndOut_Rate[i];
            New_Air_Rate[i] = copy.New_Air_Rate[i];
        }
    }




    Outdoor_Air& operator = (const Outdoor_Air& copy)
    {
        Outdoor_Air res(copy);
        return res;
    }

    virtual double get_set() { return Device::set; }
    virtual double get_mode() { return Device::mode; }
    virtual double get_status() { return Device::status; }

    virtual const string& get_version()const
    {
        return version;
    }
    const double& get_Air_I0(const int& select_mode) {
        return Air_InAndOut_Rate[set];
    }
    virtual const double New_Air()
    {
        return New_Air_Rate[set];
    }
    virtual double get_consumption() {
        if (set >= mode)
        {
            cerr << "The Selected Mode is outa of range, please check the device's mode range";
            abort();
        }
      return Device::status == 0 ? 0.0 : Device::energy_consumption[Device::set];
    }
    virtual void print_info()
    {
        cout << "------------------------------------------------------------" << endl;
        cout << "This is a Outdoor_Air_Machine, and the version is: " << version << endl;
        cout << "Air_InAndOut_Rate	New_air_rate  energy_consumption" << endl;
        for (int i = 0; i < mode; ++i)
        {
            cout << Air_InAndOut_Rate[i] << "				" << New_Air_Rate[i] << "	          " << Device::energy_consumption[i] << endl;
        }
        cout << "------------------------------------------------------------" << endl;
    }
};
//const int Outdoor_Air::kind = 2;
struct req_env {
    double temperature;
    double relative_humidity;
    req_env() :temperature(0), relative_humidity(0) {}
    req_env(double te, double rh) :temperature(te), relative_humidity(rh) {}
};
class humidifier :public Device
{
private:
    double humid_amount; //?????
    double energy_consumption;   //??????
    string version;
public:
    explicit humidifier(const int mode_) :
        Device(1, 0)  // set  humidifier's mode =1;set =0
    {}
    humidifier(const string& ver, double amount, double energy) :
        Device(1, 0), version(ver), humid_amount(amount), energy_consumption(energy) {}
    humidifier(const humidifier& copy) :Device(copy), version(copy.version),
        energy_consumption(copy.energy_consumption), humid_amount(copy.humid_amount) {}

    virtual double get_set() { return Device::set; }
    virtual double get_mode() { return Device::mode; }
    virtual double get_status() { return Device::status; }
    virtual const double get_humid_amount()const { return humid_amount; }

    virtual const string& get_version()const
    {
        return version;
    }
    virtual void print_consumption()
    {
        cout << "The consumption is :" << energy_consumption << endl;
    }
    virtual void print_info()
    {

        cout << "----------------------------" << endl;
        cout << "This is a Humidifier:" << endl
            << "The version is: " << version << endl;

        for (int i = 0; i < mode; ++i)
        {
            cout << "The humid_amount is: " << humid_amount << "ml/h" << endl
                << "And the power rate is: " << energy_consumption << "W" << endl;
        }
        cout << "----------------------------" << endl;
    }
    virtual double get_consumption() { return Device::status == 0 ? 0.0 : energy_consumption; }
    ~humidifier()
    {
        delete[]Device::energy_consumption;
    }



};
class dehumidifier :public Device
{
private:
    double* dehumid_amount; //?????
    req_env* env;           //???????????
    //const int mode;
    string version;

public:
    explicit dehumidifier(const int mode_) :Device(mode_)
    {
        env = new req_env[mode];
        dehumid_amount = new double[mode];
        energy_consumption = new double[mode];
    }
    dehumidifier(const int mode_, const string& ver, double* amount, double* energy, req_env* en) :Device(energy, mode_), version(ver)
    {
        env = new req_env[mode];
        dehumid_amount = new double[mode];
        for (int i = 0; i < mode; ++i)
        {
            env[i] = en[i];
            dehumid_amount[i] = amount[i];
        }
    }
    dehumidifier(const dehumidifier& copy) : Device(copy), version(copy.version)
    {
        dehumid_amount = new double[mode];
        env = new req_env[mode];
        for (int i = 0; i < mode; ++i)
        {
            env[i] = copy.env[i];
            dehumid_amount[i] = copy.dehumid_amount[i];
        }
    }
    ~dehumidifier()
    {
        delete[]env;
        delete[]dehumid_amount;
        delete[]energy_consumption;
    }

    virtual double get_set() { return Device::set; }
    virtual double get_mode() { return Device::mode; }
    virtual double get_status() { return Device::status; }

    virtual const string& get_version()const
    {
        return version;
    }
    virtual void print_consumption()
    {
        for (int i = 0; i < mode; ++i)
            cout << "The consumption is :" << Device::energy_consumption[i] << endl;
    }
    virtual void print_info()
    {

        cout << "---------------------------------------------" << endl;
        cout << "% % This is a Dehumidifier: " << endl
            << "The version is: " << version << endl;

        for (int i = 0; i < mode; ++i)
        {
            cout << "The dehumid_amount is: " << dehumid_amount[i] << " L/D" << endl
                << "And the power rate is: " << Device::energy_consumption[i] << " W" << endl
                << "At Relative humidity: " << env[i].relative_humidity << " and temperature: " << env[i].temperature << endl << endl;
        }
        cout << "---------------------------------------------" << endl;
    }
    virtual double get_consumption() { return Device::status == 0 ? 0.0 : energy_consumption[Device::set]; }

    virtual const double get_dehumid_amount()const { return dehumid_amount[set]; }
};
class Radiant_Floor_Heating :public Device
{
private:
    double energy_consumption = 135;  //135W per  m^2
    double Heating_power = 120; //120 W per m^2
    string ver = "Elec_FH";
public:
    Radiant_Floor_Heating() :
        Device(1, 0) {};  //mode =1; set =0;

    ~Radiant_Floor_Heating() {}
    virtual double get_set() { return Device::set; }
    virtual double get_mode() { return Device::mode; }
    virtual double get_status() { return Device::status; }

    virtual const string& get_version()const
    {
        return ver;
    }
    virtual void print_consumption()
    {
        cout << "The consumption is :" << energy_consumption << "w/m^2" << endl;
    }
    virtual void print_info()
    {
        cout << "--------------------------------" << endl;
        cout << "This is a Radiant_Floor_Heating:" << endl;
        for (int i = 0; i < mode; ++i)
        {
            cout << "The heat_power is: " << Heating_power << " w/m^2" << endl;
            print_consumption();
        }
        cout << "--------------------------------" << endl;
    }
    virtual double get_consumption() { return Device::status == 0 ? 0.0 : energy_consumption; }

    virtual const double get_heating_power()const { return Heating_power; }
};

class Device_Control {
private:
    Device* device;
public:

};

#endif // !DEVICE_H


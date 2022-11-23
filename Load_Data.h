#pragma once
#ifndef LOAD_DATA_H
#define LOAD_DATA_H
#include<iostream>
#include<unordered_map>
#include<cmath>
#include<fstream>
#include"Device.h"
using std::unordered_map;
using std::fstream;
using std::cerr;
using std::cout;
class DataSet
{
private:
    int AirConditioner_Number;
    int OutdoorAir_Number;
    int Humidifier_Number;
    int Dehumidifier_Number;
    int  Radiant_Floor_Heating_Number;
    unordered_map<string, AirConditioner*> AirConditioner_Data;
    unordered_map<string, Outdoor_Air*> OutdoorAir_Data;
    unordered_map<string, humidifier*> Humidifier_Data;
    unordered_map<string, dehumidifier*> Dehumidifier_Data;
    unordered_map<string, Radiant_Floor_Heating*> Radiant_Floor_Heating_Data;
public:
    DataSet() {}
    DataSet(int ac, int on, int hn, int dn, int rfh) :
        AirConditioner_Number(ac), OutdoorAir_Number(on), Humidifier_Number(hn),
        Dehumidifier_Number(dn), Radiant_Floor_Heating_Number(rfh)
    {
        load_AirConditioner_Data();
        load_OutdoorAir_Data();
        load_Dehumidifier_Data();
        load_Humidifier_Data();
        load_Radiant_Floor_Heating();
    }
    void load_AirConditioner_Data()
    {
        fstream f;
        f.open("..//Home_Control_System//AirConditioner.txt");
        if (!f)
        {
            cerr << "Error: Data File of AirConditioner couldn't be opened!" << endl;
            abort();
        }
        else
        {

            for (int i = 0; i < AirConditioner_Number; ++i)
            {
                string strversion;
                f >> strversion;
                double* energy_consume, heating_rate, cooling_rate, circular_wind;
                energy_consume = new double[2];

                f >> energy_consume[0] >> cooling_rate
                    >> energy_consume[1] >> heating_rate >> circular_wind;

                AirConditioner* d = new AirConditioner(strversion, energy_consume, heating_rate, cooling_rate, circular_wind);
                d->Set_Mode(0);
                /*d->Set_Mode(0);
            cout << d->get_consumption();*/
            //d->print_info();
                AirConditioner_Data[strversion] = d;

                delete[]energy_consume;
            }
        }
    }
    void show_AC()
    {
        for (auto& u : AirConditioner_Data)
        {
            u.second->print_info();
        }
    }
    unordered_map<string, AirConditioner*> get_AirConditiioner_Data()
    {
        return AirConditioner_Data;
    }
    void load_OutdoorAir_Data()
    {
        fstream f;
        f.open("..//Home_Control_System//OurdoorAir.txt");
        if (!f)
        {
            cerr << "Error: Data File of OutdoorAir couldn't be opened!" << endl;
            abort();
        }
        else
        {
            //cout << "Succsefully connected to txt data file..." << endl;
            for (int i = 0; i < OutdoorAir_Number; ++i)
            {
                string strversion;
                int mode;
                f >> strversion >> mode;
                //cout << strversion << mode << endl;
                double* energy_consume, * Air_IO, * New_Air;
                energy_consume = new double[mode];
                Air_IO = new double[mode];
                New_Air = new double[mode];

                for (int j = 0; j < mode; ++j)
                {
                    f >> energy_consume[j] >> Air_IO[j] >> New_Air[j];
                    //cout << energy_consume[j]<<" " << Air_IO[j]<<" " << New_Air[j] << endl;
                }

                Outdoor_Air* d = new Outdoor_Air(mode, Air_IO, energy_consume, New_Air, strversion);
                //Outdoor_Air(const int mode_, double* In, double* Energy, double* New_Air,const string version_)
                    /*d->Set_Mode(0);
                cout << d->get_consumption();*/
                //d->print_info();
                OutdoorAir_Data[strversion] = d;

                delete[]energy_consume;
                delete[]Air_IO;
                delete[]New_Air;
            }
        }

    }
    void show_OA()
    {
        for (auto& u : OutdoorAir_Data)
        {
            u.second->print_info();
        }
    }
    unordered_map<string, Outdoor_Air*> get_OutdoorAir_Data()
    {
        return  OutdoorAir_Data;
    }
    void load_Humidifier_Data()
    {
        fstream f;
        f.open("..//Home_Control_System//Humidifier.txt");
        if (!f)
        {
            cerr << "Error: Data File of Humidifier couldn't be opened!" << endl;
            abort();
        }
        else
        {
            cout << "Succsefully connected to txt data file..." << endl;
            for (int i = 0; i < Humidifier_Number; ++i)
            {
                string ver;
                double energy_consume, amount;

                f >> ver >> energy_consume >> amount;
                humidifier* d = new  humidifier(ver, amount, energy_consume);
                //humidifier(const string& ver, double amount,double* energy)
                Humidifier_Data[ver] = d;
            }

        }
    }
    void show_HD()
    {
        for (auto& u : Humidifier_Data)
        {
            u.second->print_info();
        }
    }
    unordered_map<string, humidifier*> get_Humidifier_Data()
    {
        return Humidifier_Data;
    }
    void load_Dehumidifier_Data()
    {
        fstream f;
        f.open("..//Home_Control_System//Dehumidifier.txt");
        if (!f)
        {
            cerr << "Error: Data File of Dehumidifer couldn't be opened!" << endl;
            abort();
        }
        else
        {
            cout << "Succsefully connected to txt data file..." << endl;
            for (int i = 0; i < Dehumidifier_Number; ++i)
            {
                string ver;
                int mode;
                f >> ver >> mode;
                double* energy_consume, * amount;
                req_env* env;
                energy_consume = new double[mode];
                amount = new double[mode];
                env = new req_env[mode];
                for (int j = 0; j < mode; ++j)
                {
                    double tep, rh;
                    f >> amount[j] >> energy_consume[j] >> tep >> rh;
                    env[j] = req_env(tep, rh);
                }

                dehumidifier* d = new  dehumidifier(mode, ver, amount, energy_consume, env);
                //dehumidifier(const int mode_, const string& ver, double* amount,double* energy, req_env* en)
                Dehumidifier_Data[ver] = d;
            }

        }
    }
    void show_DD()
    {
        for (auto& u : Dehumidifier_Data)
        {
            u.second->print_info();
        }
    }
    unordered_map<string, dehumidifier*> get_Dehumidifier_Data()
    {
        return Dehumidifier_Data;
    }
    void load_Radiant_Floor_Heating()
    {
        Radiant_Floor_Heating* d = new Radiant_Floor_Heating();
        string ver = d->get_version();
        d->Set_Statues(1);
        Radiant_Floor_Heating_Data[ver] = d;
    }
    void show_RFH()
    {
        for (auto& u : Radiant_Floor_Heating_Data)
        {
            u.second->print_info();
        }
    }
    unordered_map<string, Radiant_Floor_Heating*> get_Radiant_Floor_Heating_Data()
    {
        return Radiant_Floor_Heating_Data;
    }
};

double Absolute_Humidity(double T, double RH, double P = 1013.25)
{
    double x = 17.62 * T / (243.12 + T);
    double fp = 1.0016 + 3.15e-6 * P - 0.074 / P;
    double ew = 6.112 * exp(x) * fp;
    double e = ew * RH * 100;
    double AH = e / (461.5 * (T + 273.15)) * 1000;
    return AH;
    //g/m^3

}
double Relative_Humidity(double T, double AH, double P = 1013.25)
{
    double x = 17.62 * T / (243.12 + T);
    double fp = 1.0016 + 3.15e-6 * P - 0.074 / P;
    double ew = 6.112 * exp(x) * fp;
    double	Z = (461.5 * (T + 273.15)) / 1000;
    double RH = AH * Z / (ew * 100);
    return RH;

}

#endif // !LOAD_DATA_H

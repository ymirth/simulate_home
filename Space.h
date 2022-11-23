#pragma once
#pragma once
#ifndef SPACE_H
#define SPACE_H
#include<iostream>
#include<vector>
#include<sstream>
#include"Load_Data.h"
#include"Device.h"
#include"environment.h"
#include<iomanip>
using std::cin;
using std::vector;
#define TEMPERATURE_MIN_AC 16
#define HUMIDITY_LOWER_BOUND 0.4
#define HUMIDITY_UPPER_BOUND 0.5

# define AC 4
# define OA 3
# define HN 2
# define DN 3
# define RFH 1
using device_order = int;
const string air_conditoner_version[AC] = { "KFR-52L2","KFR-72L2" ,"KFR-26GW" ,"KFR-35GW" };
const string new_air_version_arr[OA] = { "FY-15ZM1C","FY-25ZM1C","FY-35ZM1C" };
const string dehumidifier_arr[DN] = { "F-CYU60XC","F-YCJ17C-X","F-YCJ10C-W" };
const string humidifier_arr[HN] = { "XIAOMIpro","skin" };
const string radiant_floor_heating[1] = { "Elec_FH" };
unordered_map<device_order, string> device_name_hash
{ {1,"air conditoiner"},{2,"new air machine"},{3,"humidifier"},{4,"dehumidifier"},{5,"floor_heating"} };

DataSet D(AC, OA, HN, DN, RFH);
vector<double> co2_generation_speed_per_hour_arr{ 0.0144,0.0173,0.023,0.0748 };

std::string convertDoubleToString(const long double value, const int precision = 0)
{
    std::stringstream stream{};
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

class Window
{
private:
    double new_air;				// m^3/h
    double area;
    bool status;
public:
    Window() :area(0), status(1) {};
    explicit Window(double a) : area(a), status(1)
    {
        load_new_air();
    }
    Window(const Window& copy) :
        new_air(copy.new_air), area(copy.area), status(copy.status) {}
    ~Window() {};

    Window& operator =(const Window copy)
    {
        if (this != &copy) {  //?ж?????????????
            this->new_air = copy.new_air;
            this->area = copy.area;
            this->status = copy.status;
        }
        return *this;
    }
    //const double& Heat_Consume() { return heat_consume; }
    const double& New_Air() { return new_air; }

    void load_new_air()
    {
        new_air = area * air.air_speed();// * 0.02;
    }

};
class Wall
{
private:
    double heat_consume;
    double area;
    static const double convective_heat_transfer_coefficient;//W/(m^2????)

    double caculate_heat_consume()
    {
        double c = convective_heat_transfer_coefficient * area;
        return c;
    }

public:
    Wall() : area(0)
    {
        heat_consume = caculate_heat_consume();
    };
    explicit Wall(double a):area(a)
    {
        heat_consume = caculate_heat_consume();
    }
    Wall(const Wall &copy)
    {
        this->heat_consume = copy.heat_consume;
        this->area = copy.area;
    }
    ~Wall() {};

    Wall& operator =(const Wall& copy)
    {
        if (&copy == this) return *this;
        this->heat_consume = copy.heat_consume;
        this->area = copy.area;
        return *this;
    }
    Wall& operator+(const Wall& adder)
    {
        this->heat_consume += adder.heat_consume;
        this->area += adder.heat_consume;
        return *this;
    }

    void set_hc(double c) { heat_consume = c; }
    void set_area(double a) { area = a; }

    const double& Heat_Consume() { return heat_consume; }
    const double& get_area() { return area; }
};
const double Wall::convective_heat_transfer_coefficient = 0.37;

class Room
{
protected:
    double room_space;										//房间面积		m^2
    double temperature;										//房间温度
    double co2_concenration;								//co2浓度  m^3
    double room_volume;										//房间体积   m^3
    double humidity;										//房间水汽含量  g
    double AH;												//房间绝对湿度
    double RH;												//房间相对湿度
    int device_num = 0;										//房间仪器数量
    const int device_max_num;								//房间仪器最大数量

    Device** device_arr = nullptr;							//设备数组
    unordered_map<device_order, int> device_hash;
    static int time;										//时间

    int window_num = 1;
    friend class Room_Control;
    friend class Home;

    Window* windows = nullptr;
    Wall* walls = nullptr;
public:
    Room(const int& num) :device_max_num(num), temperature(27),
        co2_concenration(12e-4)
    {
        if (num != 0)
        {
            device_arr = new Device * [device_max_num];
            select_device();
            init_room_features();
        }
        init_room_features();
        init_window();
        walls = new Wall(room_volume / room_space * 30);
    }
    Room(const int& num, const double temp_, const double& co2_) :
        device_max_num(num), temperature(air.air_temperature()),
        co2_concenration(co2_)
    {
        device_arr = new Device * [device_max_num];
        init_room_features();
        init_window();
        walls = new Wall(room_volume / room_space * 30);
        select_device();
    }
    Room(const int num, const double room_space_, const double room_volume_, const double temp_, const double co2_) :
        room_space(room_space_), temperature(temp_),
        co2_concenration(co2_), device_max_num(num), room_volume(room_volume_)
    {
        //device_arr = new Device * [device_max_num];
        //init_window();
        //select_device();  // add devices
    }
    Room(const int num, const double room_space_, const double room_volume_, const double temp_, const double co2_,const int win_n) :
        room_space(room_space_), temperature(temp_),
        co2_concenration(12e-4), device_max_num(num), room_volume(room_volume_),
        window_num(win_n)
    {
        RH = air.air_rh_humidity();
        AH = Absolute_Humidity(temperature, RH);
        humidity = AH * room_volume;
        device_arr = new Device * [device_max_num];
        select_device();
    }
    ~Room()
    {
        /*	for (int i = 0; i < device_num; ++i)
            {
                delete[]device_arr[i];

            }*/
        delete walls;
        delete[]windows;
    }


    /*      Query          */
    /*const double& get_Fresh_Air_Stream() { return fresh_air_stream; }*/
    const double& get_Temprature() { return temperature; }
    const double& get_room_space() { return room_space; }
    const double& get_co2() { return co2_concenration; }
    const double& get_room_volume() { return room_volume; }
    /*const double& get_cooling_power() { return cooling_power; }
    const double& get_heating_power() { return heating_power; }
    const double& get_humid_amount() { return humid_amount; }
    const double& get_dehumid_amount() { return dehumid_amount; }*/



private:

    void set_room_feature()
    {
        cout << "------------------------" << endl;
        double f1, f2, f3, f4;
        cout << "Please input the room's space: " << endl;
        cin >> f1;
        room_space = f1;
        cout << "Please input the room's volume: " << endl;
        cin >> f2;
        room_volume = f2;
        cout << "Please input the room's initial temperature: " << endl;
        cin >> f3;
        temperature = f3;
        cout << "Please input the room's initial co2 concentration: " << endl;
        cin >> f4;
        co2_concenration = f4;
        cout << "------------------------" << endl;
    }


    /*		Operation		*/
    void device_menu()
    {
        cout << "Choose the device type by inputting the number in the following list: " << endl;
        cout << "++-----------------------------------------------------------------------------------------------------------------++" << endl;
        cout << "|| ";
        cout << "Air Conditioner  " << "1" << "	  ";
        cout << "Outdoor air conditioner " << "2" << "	";
        cout << " Humidifier " << "3" << "	";
        cout << "Dehumidifier " << "4" << "	";
        cout << "Radiant_Floor_Heating " << "5   ";
        cout << " ||" << endl;
        cout << "++-----------------------------------------------------------------------------------------------------------------++" << endl;
    }
    void wall_menu()
    {
        cout << "Choose the device type by inputting the number in the following list: " << endl;
    }
    void select_device()
    {
        static int floor_flag = 0;
        device_menu();
        int k = 0;
        while (device_num < device_max_num)
        {
            int select;
            cout << "Please input the type number: ";
            cin >> select;
            cout << endl;
            switch (select)
            {
            case 1:
            {
                D.show_AC();
                cout << "------------------------------------------" << endl;
                cout << "|| ";
                for (auto& u : air_conditoner_version)
                    cout << u << " ";
                cout << " ||" << endl;
                cout << "------------------------------------------" << endl;

                int version_tag;
                cout << "Please choose the version by 1 -- " << AC << " : ";
                cin >> version_tag;
                string version = air_conditoner_version[version_tag - 1];
                device_arr[device_num] = D.get_AirConditiioner_Data()[version];
                ++device_num;
                device_hash[1] = k;
                //device_arr[device_num]->print_info();
                break;
            }
            case 2:
            {
                D.show_OA();
                cout << "|| ";
                for (auto& u : new_air_version_arr)
                    cout << u << " ";
                cout << " ||";

                int version_tag;
                cout << "Please choose the version by 1 -- " << OA << " : ";
                cin >> version_tag;
                string version = new_air_version_arr[version_tag - 1];
                device_arr[device_num] = D.get_OutdoorAir_Data()[version];
                device_arr[device_num]->print_info();
                ++device_num;
                device_hash[2] = k;
                break;
            }
            case 3:
            {
                D.show_HD();
                cout << "|| ";
                for (auto& u : humidifier_arr)
                    cout << u << " ";
                cout << " ||";

                int version_tag;
                cout << "Please choose the version by 1 -- " << HN << " : ";
                cin >> version_tag;
                string version = humidifier_arr[version_tag - 1];
                device_arr[device_num] = D.get_Humidifier_Data()[version];
                device_arr[device_num]->print_info();
                ++device_num;
                device_hash[3] = k;
            }
            break;
            case 4:
            {
                D.show_DD();
                cout << "|| ";
                for (auto& u : dehumidifier_arr)
                    cout << u << " ";
                cout << " ||";

                int version_tag;
                cout << "Please choose the version by 1 -- " << DN << " : ";
                cin >> version_tag;
                string version = dehumidifier_arr[version_tag - 1];
                device_arr[device_num] = D.get_Dehumidifier_Data()[version];
                device_arr[device_num]->print_info();
                device_hash[4] = k;
                ++device_num;
            }break;
            case 5:
            {
                if (floor_flag == 0)
                {
                    floor_flag = 1;
                    D.show_RFH();
                    cout << "|| ";
                    for (auto& u : radiant_floor_heating)
                        cout << u << " ";
                    cout << " ||";

                    int version_tag;
                    cout << "Please choose the version by 1 -- " << RFH << " : ";
                    cin >> version_tag;
                    string version = radiant_floor_heating[version_tag - 1];
                    device_arr[device_num] = D.get_Radiant_Floor_Heating_Data()[version];
                    device_arr[device_num]->print_info();
                    ++device_num;
                    device_hash[5] = k;
                }
                else
                {
                    cout << "Already have floor heating" << endl;
                }

            }
            break;

            }
            ++k;
            if (device_num < device_max_num)
            {
                int add_choice;
                cout << "Continue adding input 1, else input 0 : ";
                cin >> add_choice;
                if (add_choice == 0) break;
                else
                {
                    device_menu();
                    continue;
                }
            }
            else break;


        }

    }
    void init_room_features()
    {
        cout << "Pleas input the room's space" << endl;
        cin >> room_space;
        cout << "Please input the room's volume" << endl;
        cin >> room_volume;

        RH = air.air_rh_humidity();
        AH = Absolute_Humidity(temperature, RH);
        humidity = AH * room_volume;
    }
    void init_window()
    {
        int a;
        cout << "Please input the  room's windows area: ";
        cin >> a;
        windows = new Window(a);
    }
    void init_wall()
    {
        int a;
        cout << "Please input the  room's windows area: ";
        cin >> a;
        walls = new Wall(a);
    }

};


class Room_Control {
private:
    double co2_generation_rate;
    double fresh_air_stream;							//房间新风量
    double cooling_power;								//房间制冷量	w
    double heating_power;								//房间制热量	w
    double humid_amount;								//房间加湿量  g/s
    double dehumid_amount;								//房间除湿量  g/s
    double energy_consumption;
    Room* room;

    void load_fresh_air()
    {
        if (fresh_air_stream != 0) fresh_air_stream = 0;
        for (int i = 0; i < room->device_num; ++i)
            if (room->device_arr[i]->get_status() != 0)
                fresh_air_stream += room->device_arr[i]->New_Air();

        for(int i=0;i<room->window_num;++i)
            fresh_air_stream += room->windows->New_Air();
    }
    void load_cooling_power()
    {
        if (cooling_power != 0) cooling_power = 0;
        for (int i = 0; i < room->device_num; ++i)
        {
            if (room->device_arr[i]->get_status() != 0)
                cooling_power += room->device_arr[i]->get_cooling_power();
        }
    }
    void load_heating_power()
    {
        if (heating_power != 0) heating_power = 0;
        for (int i = 0; i < room->device_num; ++i)
        {
            if (room->device_arr[i]->get_status() != 0)
            {
                if (room->device_arr[i]->get_heating_power() > 300)
                    heating_power += room->device_arr[i]->get_heating_power();
                else 	heating_power += room->device_arr[i]->get_heating_power() * room->get_room_space();
            }
        }

    }
    void load_humid_amount()
    {
        if (humid_amount != 0) humid_amount = 0;
        for (int i = 0; i < room->device_num; ++i)
            if (room->device_arr[i]->get_status() != 0)
                humid_amount += room->device_arr[i]->get_humid_amount();   // unit: ml/h
        humid_amount = humid_amount / 3600;                      // unit: g/s
    }
    void load_dehumid_amount()
    {
        if (dehumid_amount != 0) dehumid_amount = 0;
        for (int i = 0; i < room->device_num; ++i)
            if (room->device_arr[i]->get_status() != 0)
                dehumid_amount += room->device_arr[i]->get_dehumid_amount(); //unit: l/d
        dehumid_amount = dehumid_amount * 1000 / (3600.0 * 24);    //unit: g/s
    }

public:
    Room_Control(Room* rcp) :room(rcp), energy_consumption(0.0)
    {
        int k;
     /*   cout << "Do you want to close some deivce ? " << endl
            << "1: yes " << "2: no " << endl;
        cin >> k;
        if (k == 1)
            set_device();*/
        refreash();
    }

private:
    void refreash()
    {
        load_fresh_air();
        load_cooling_power();
        load_heating_power();
        load_humid_amount();
        load_dehumid_amount();
    }
    void set_device()
    {
        unordered_map<device_order, string>::iterator iter;
        for (int i = 0; i < room->device_hash.size(); ++i)
        {
            for (auto& u : room->device_hash)
            {
                int temp = u.first;
                iter = device_name_hash.find(temp);
                if (room->device_arr[u.second]->get_status() != 0)
                    cout << u.second << ": " << iter->second << "  ";
            }
            int n;
            cout << endl << "Please select the device to turn off : ";
            cin >> n;
            Device* control_ptr = room->device_arr[n];
            control_ptr->Set_Statues(0);
            cout << "continue to turn off,pleas input 1, else input 0 :";
            bool flag;
            cin >> flag;
            if (flag == 0) break;

        }
    }


    void predict_co2_graph_data_to_textfile()
    {
        FILE* fp;
        fopen_s(&fp, "C:\\Users\\mirth\\Desktop\\data.txt", "w");
        for (int i = 0; i < 200000; ++i)
        {
            room->co2_concenration = predict_co2_per_second(co2_generation_speed_per_hour_arr[3] / 3600);
            if (i % 10000 == 0)
            {
                fprintf(fp, "%f ", double(i));
                fprintf(fp, "%f\n", room->co2_concenration);
                //cout << co2_concenration << endl;
            }
        }
        fclose(fp);
    }
    double predict_cooling_temperature_per_second()
    {
        double C = air.air_C(); //   kj/(m^3 *K)
        double wall_consume = room->walls->Heat_Consume() * (air.air_temperature() - room->temperature);
        //cout << "Wall_consume = " << wall_consume << endl;
        double delta_T_P = ((-cooling_power) + wall_consume + heating_power) / (C * room->room_volume * 1000);
        return delta_T_P;
    }
    void auto_control_temperature(double set_t, double Input_New_Air_Amount = 0)
    {
        if (Input_New_Air_Amount == 0) Input_New_Air_Amount = fresh_air_stream / 3600;
        double C = air.air_C(); //   kj/(m^3 *K)
        int finished_time = 0;
        FILE* fp;
        fopen_s(&fp, "C:\\Users\\mirth\\Desktop\\dataTemp.txt", "w");
        for (int i = room->time; i < 200000; ++i)
        {
            if (room->temperature > set_t && room->temperature > TEMPERATURE_MIN_AC)
            {
                if (cooling_power == 0 && room->temperature > set_t + 0.3)

                    load_cooling_power();
            }
            else
            {

                if (cooling_power != 0)cooling_power = 0;
                finished_time = i;
            }
            //cout <<"temp0 : "<< temperature << endl;
            //cout << i <<" : ";
            //cout << "new air: " << Input_New_Air_Amount << " ";

            room->temperature = (
                (room->temperature + 273) * (room->room_volume - Input_New_Air_Amount) * C   //房间内空气热量
                + (air.air_temperature() + 273) * (Input_New_Air_Amount)*C                   //输入空气热量
                )
                / (C * room->room_volume) - 273;                                             //每秒钟换气后空气的热量
            room->temperature += predict_cooling_temperature_per_second();                   //+空调/地暖 + 墙体的消耗量
            fprintf(fp, "%f ", double(i));
            fprintf(fp, "%f\n", room->temperature);
            //cout << temperature << endl;

        }
        fclose(fp);
        cout << finished_time;
    }
    void predict_humidity_data_to_textfile()
    {

        FILE* fp;
        fopen_s(&fp, "C:\\Users\\mirth\\Desktop\\data_humidity.txt", "w");
        for (int i = 0; i < 2000000; ++i)
        {
            predict_humidity_per_second();
            if (i % 10000 == 0)
            {
                fprintf(fp, "%f ", double(i));
                fprintf(fp, "%f\n", room->RH);
                //cout << co2_concenration << endl;
            }


            //cout << i << " : " << room->RH << endl;
        }


        fclose(fp);
    }


    int set_ac(int set_t)
    {
        if (room->temperature > TEMPERATURE_MIN_AC && room->temperature > set_t)
        {
            unordered_map<device_order, int>::iterator it;
            if ((it = room->device_hash.find(5)) != room->device_hash.end())
            {
                Device* d = room->device_arr[it->second];
                d->Set_Statues(0);
            }
            if ((it = room->device_hash.find(1)) != room->device_hash.end())
            {
                Device* d = room->device_arr[it->second];
                d->Set_Mode(0);
                return it->second;
            }
            else return -1;
        }
        else if (room->temperature < TEMPERATURE_MIN_AC)
        {
            unordered_map<device_order, int>::iterator it;
            if ((it = room->device_hash.find(5)) != room->device_hash.end())
            {
                Device* d = room->device_arr[it->second];
                d->Set_Statues(1);
            }
            if ((it = room->device_hash.find(1)) != room->device_hash.end())
            {
                Device* d = room->device_arr[it->second];
                d->Set_Mode(1);

                return it->second;
            }
            else return -1;
        }
        /*  else
          {
              unordered_map<device_order, int>::iterator it;
              if ((it = room->device_hash.find(1)) != room->device_hash.end())
              {
                  Device* d = room->device_arr[it->second];
                  d->Set_Statues(0);
                  return it->second;
              }
              else return -1;
          }*/
    }
    void control_ac(int ac, int set_t)
    {

        if (ac >= 0) {
            if (room->device_arr[ac]->get_set() == 0)
            {
                if (room->temperature > set_t && room->temperature > TEMPERATURE_MIN_AC)
                {

                    if (room->temperature > set_t + 1 && room->device_arr[ac]->get_status() == 0)
                    {
                        room->device_arr[ac]->Set_Statues(1);       //空调工作
                        room->device_arr[ac]->Set_Mode(0);
                    }
                }
                else
                {
                    if (room->device_arr[ac]->get_status() != 0)
                        room->device_arr[ac]->Set_Statues(0);  //空调停止
                    /*cout << room->device_arr[ac]->get_status();*/
                }

            }
            else if (room->device_arr[ac]->get_set() == 1)
            {
                if (room->temperature < set_t || room->temperature < TEMPERATURE_MIN_AC)
                {
                    if (heating_power == 0 && room->temperature < set_t - 1)
                    {
                        room->device_arr[ac]->Set_Statues(1);   //空调工作
                        room->device_arr[ac]->Set_Mode(1);
                    }
                }
                else
                {
                    if (heating_power != 0) room->device_arr[ac]->Set_Statues(0);  //空调停止
                }


                load_cooling_power();
            }
            else return;

        }
    }
public:

    double predict_co2_per_second(double gernerating_co2_speed, double Input_New_Air_Amount = 0, double co2_concentartion_of_new_air = air.co2_concentration())
    {
        //gernerating_co2_speed  (ppm*1000000) /s
        //double co2_concenration_p = gernerating_co2_speed / Input_New_Air_Amount + co2_concentartion_of_new_air;
        if (Input_New_Air_Amount == 0) Input_New_Air_Amount = fresh_air_stream / 3600;
        double co2_concenration_p = (gernerating_co2_speed / 3600 + co2_concentartion_of_new_air * Input_New_Air_Amount
            + room->co2_concenration * (room->room_volume - Input_New_Air_Amount)) / room->room_volume;
        room->co2_concenration = co2_concenration_p;
        return co2_concenration_p;
    }
    void predict_temperature_per_second(double Input_New_Air_Amount = 0)
    {
        if (Input_New_Air_Amount == 0) Input_New_Air_Amount = fresh_air_stream / 3600;
        Air* a = &air;
        double C = air.air_C(); //   kj/(m^3 *K)
        room->temperature = (
            (room->temperature + 273) * (room->room_volume - Input_New_Air_Amount) * C   //房间内空气热量
            + (air.air_temperature() + 273) * (Input_New_Air_Amount)*C                   //输入空气热量
            )
            / (C * room->room_volume) - 273;
        //每秒钟换气后空气的热量
        room->temperature += predict_cooling_temperature_per_second();                   //+空调/地暖 + 墙体的消耗量
    }
    double predict_humidity_per_second(double Input_New_Air_Amount = 0)
    {
        double ah_of_air = Absolute_Humidity(air.air_temperature(), air.air_rh_humidity());  //unit: g/m^3
        if (Input_New_Air_Amount == 0) Input_New_Air_Amount = fresh_air_stream / 3600;
        double inputWater = ah_of_air * Input_New_Air_Amount;
        double originalWater = room->AH * (room->room_volume - Input_New_Air_Amount);
        room->AH = (
            inputWater														// input water : g
            + originalWater													// original water
            + humid_amount                                                  // produce water
            - dehumid_amount												//reduce water
            ) / room->room_volume;
        room->RH = Relative_Humidity(room->temperature, room->AH) >= 1 ? 1 : Relative_Humidity(room->temperature, room->AH);
        return room->RH;
    }


    void control_hu()
    {
        if (room->RH > HUMIDITY_UPPER_BOUND)
        {
            unordered_map<device_order, int>::iterator it;
            if ((it = room->device_hash.find(3)) != room->device_hash.end())
            {
                Device* d = room->device_arr[it->second];
                d->Set_Statues(0);
               /* return it->second;*/
            }
            if ((it = room->device_hash.find(4)) != room->device_hash.end())
            {
                Device* d = room->device_arr[it->second];
                d->Set_Statues(1);
                //return it->second;
            }
        }
        else if (room->RH < HUMIDITY_LOWER_BOUND)
        {
            unordered_map<device_order, int>::iterator it;
            if ((it = room->device_hash.find(3)) != room->device_hash.end())
            {
                Device* d = room->device_arr[it->second];
                d->Set_Statues(1);
                 //return it->second;
            }
            if ((it = room->device_hash.find(4)) != room->device_hash.end())
            {
                Device* d = room->device_arr[it->second];
                d->Set_Statues(0);
                /*return it->second;*/
            }
        }
    }
    void consume_enengy_consumption()
    {
        int size = room->device_num;
        for (int i = 0; i < size; ++i)
        {
            energy_consumption += room->device_arr[i]->get_consumption();
        }
    }

    void auto_control(int set_t)
    {
        FILE* fp1, * fp2, * fp3, * fp4;
        fopen_s(&fp1, "C:\\Users\\mirth\\Desktop\\data_temperature.txt", "w");
        fopen_s(&fp2, "C:\\Users\\mirth\\Desktop\\data_humidity.txt", "w");
        fopen_s(&fp3, "C:\\Users\\mirth\\Desktop\\data_co2.txt", "w");
        fopen_s(&fp4, "C:\\Users\\mirth\\Desktop\\data_air.txt", "w");

        set_ac(set_t);
        int ac = room->device_hash[1];
        cout << "ac: " << ac << endl;
        for (int i = 0; i < 86400; ++i)
        {
            simulate_temperature(i);
            predict_co2_per_second(co2_generation_speed_per_hour_arr[3] / 3600);
            predict_temperature_per_second();
            predict_humidity_per_second();

            fprintf(fp4, "%f ", double(i));

                fprintf(fp4, "%lf\n", air.air_temperature());
            if (i % 10000 == 0)
            {
                fprintf(fp3, "%f ", double(i));
                fprintf(fp3, "%lf\n", room->co2_concenration);
                //cout << convertDoubleToString(room->co2_concenration, 6) << endl;
            }

            fprintf(fp2, "%f ", double(i));
            fprintf(fp2, "%lf\n", room->RH);

            fprintf(fp1, "%f ", double(i));

                fprintf(fp1, "%lf\n", room->temperature);



            set_ac(set_t);
            control_ac(ac, set_t);                // control  temperature
            control_hu();                         // control  humidity
            consume_enengy_consumption();         // compute  consumption
            refreash();
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(fp4);

        cout << "energy_consumption is: " << energy_consumption/1000/3600<<" degree " << endl;

    }
    void part_control(int set_t, int i)
    {
        FILE* fp1, * fp2, * fp3, * fp4;
        fopen_s(&fp1, "C:\\Users\\mirth\\Desktop\\data_temperature.txt", "w");
        fopen_s(&fp2, "C:\\Users\\mirth\\Desktop\\data_humidity.txt", "w");
        fopen_s(&fp3, "C:\\Users\\mirth\\Desktop\\data_co2.txt", "w");
        fopen_s(&fp4, "C:\\Users\\mirth\\Desktop\\data_air.txt", "w");

        set_ac(set_t);
        int ac = room->device_hash[1];
        cout << "ac: " << ac << endl;

        /////////////////////////////////////////////////////////////////////
        simulate_temperature(i);
        predict_co2_per_second(co2_generation_speed_per_hour_arr[3] / 3600);
        predict_temperature_per_second();
        predict_humidity_per_second();

        fprintf(fp4, "%f ", double(i));

        fprintf(fp4, "%lf\n", air.air_temperature());
        if (i % 10000 == 0)
        {
            fprintf(fp3, "%f ", double(i));
            fprintf(fp3, "%lf\n", room->co2_concenration);
            //cout << convertDoubleToString(room->co2_concenration, 6) << endl;
        }

        fprintf(fp2, "%f ", double(i));
        fprintf(fp2, "%lf\n", room->RH);

        fprintf(fp1, "%f ", double(i));

        fprintf(fp1, "%lf\n", room->temperature);



        set_ac(set_t);
        control_ac(ac, set_t);                // control  temperature
        control_hu();                         // control  humidity
        consume_enengy_consumption();         // compute  consumption
        refreash();
        /////////////////////////////////////////////////////////////////////
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(fp4);

        cout << "energy_consumption is: " << energy_consumption / 1000 / 3600 << " degree " << endl;
    }


    Room* get_room() { return room; }
};


class Home
{
private:
    Room_Control** Controler;
    Room* home;
    int room_number;
    static int time;
    friend class Home_Controler;
public:
    Home() {
        cout << "Please input the room number: ";
        cin >> room_number;
        double space_t = 0.0, volume_t = 0.0;

        Controler = new Room_Control * [room_number];
        for (int i = 0; i < room_number; ++i)
        {
            cout << "ROOM " << i << " : " << endl;
            Room* temp;

            temp = new Room(0);
            Controler[i] = new Room_Control(temp);
            space_t+=temp->get_room_space();
            volume_t += temp->get_room_volume();

        }
        int x;
        cout << "Please input the number of device that the room can hold : ";
        cin >> x;
        home = new Room(x, space_t, volume_t, air.air_temperature(), air.co2_concentration(),room_number);
        for (int i = 0; i < room_number; ++i)
        {
            Room* temp = Controler[i]->get_room();
            Wall* wall_t = temp->walls;
            Window* win_t = temp->windows;
            if (i == 0)
            {
                home->walls = new Wall();
                home->windows = new Window[room_number];
            }
            home->walls = &((*home->walls) + (*wall_t));
            home->windows[i] = Window(*win_t);
        }
    }
    ~Home() {}


};
int Home::time = 0;

class Home_Controler {
private:
    Room_Control* Controler;
public:
    Home_Controler(){}
    bool operator()(Home* Building,int set_t)
    {
        Controler = new Room_Control(Building->home);
        Controler->auto_control(set_t);
        return 1;
    }
};


#endif // !SPACE_H

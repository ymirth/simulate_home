#include "widget.h"
#include<iostream>
#include<string>
#include<unordered_map>
#include"Space.h"
#include"Device.h"
#include"Load_Data.h"
#include <QDebug>
using namespace std;

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
int Factory(int argc, char *argv[])
{

    /*DataSet D(AC, OA, HN, DN,RFH);
    D.show_OA();
    D.show_AC();
    D.show_DD();
    D.show_HD();*/
//    int d1;
//    cout << "Please input the number of devices: " << endl;
//    cin >> d1;
//    Room room1(d1, 27, 12e-4);
//    Room_Control con(&room1);

//    con.auto_control(19);
//    cout << "Finished" << endl;

    Home* building = new Home();
        Home_Controler controler;
        controler(building, 19);
    std::cout<<"Hello+++++2"<<std::endl;

     QApplication a(argc, argv);
     QMainWindow window;
     ThemeWidget *widget = new ThemeWidget();
     window.setCentralWidget(widget);
     window.resize(1500, 900);
     window.show();
     return a.exec();
}
int main(int argc, char *argv[])
{

//    double ee[2] = { 1, 2 };
//    Device* d = new AirConditioner("v1",ee, 20, 20, 20);
//    d->print_info();
     Factory(argc, argv);
     std::cout<<"Hello+++++3"<<std::endl;


}




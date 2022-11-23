#include "widget.h"
#include "ui_widget.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui_ThemeWidgetForm)
{
    m_ui->setupUi(this);


    //create charts
    //std::cout<<"the chart funciont 0:"<<std::endl;
    //createSplineChart();

    createSplineChart_of_temperature();
    createSplineChart_of_co2();
    createSplineChart_of_humidity();
    createSplineChart_of_air();
//    QChartView *chartView;
//    chartView = m_ui->chartview;
//    m_charts << chartView;
    //std::cout<<"the chart funciont 2:"<<std::endl;
    // Set defaults


    // Set the colors from the light theme as default ones
    QPalette pal = qApp->palette();
//    pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
//    pal.setColor(QPalette::WindowText, QRgb(0x404044));
    pal.setColor(QPalette::Window, QRgb(0x121218));
    pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    qApp->setPalette(pal);


}

ThemeWidget::~ThemeWidget()
{
    delete m_ui;
}

DataList ThemeWidget::getdata_Temperature()
{
    QList<QPointF> dataList;
    qreal yValue(0);

    QFile file("C:\\Users\\mirth\\Desktop\\data_temperature.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))   // 文件打开失败
       { std::cerr<<"Chart data file could not open";
        return dataList;
    }

    while (!file.atEnd())
    {
        QString line = file.readLine();
        QStringList strList = line.split(" ", Qt::SkipEmptyParts);  // 以空格拆分字符串

        double x =strList.at(0).toDouble();
         yValue =strList.at(1).toDouble();

//        std::cout<<"x is "<<x<<" and y is "<<yValue<<std::endl;
        QPointF value(x,yValue);
        dataList << Data(value);
    }

    return dataList;
}

DataList ThemeWidget::getdata_co2()
{

    QList<QPointF> dataList;
    qreal yValue(0);

    QFile file("C:\\Users\\mirth\\Desktop\\data_co2.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))   // 文件打开失败
       { std::cerr<<"Chart data file could not open";
        return dataList;
    }

    while (!file.atEnd())
    {
        QString line = file.readLine();
        QStringList strList = line.split(" ", Qt::SkipEmptyParts);  // 以空格拆分字符串

        double x =strList.at(0).toDouble();
         yValue =strList.at(1).toDouble();

        //std::cout<<"x is "<<x<<" and y is "<<yValue<<std::endl;
        QPointF value(x,yValue);
        dataList << Data(value);
    }

    return dataList;
}

DataList ThemeWidget::getdata_Humidity()
{

    QList<QPointF> dataList;
    qreal yValue(0);

    QFile file("C:\\Users\\mirth\\Desktop\\data_humidity.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))   // 文件打开失败
       { std::cerr<<"Chart data file could not open";
        return dataList;
    }


    while (!file.atEnd())
    {
        QString line = file.readLine();
        QStringList strList = line.split(" ", Qt::SkipEmptyParts);  // 以空格拆分字符串

        double x =strList.at(0).toDouble();
         yValue =strList.at(1).toDouble();


//        std::cout<<"x is "<<x<<" and y is "<<yValue<<std::endl;
        QPointF value(x,yValue);
        dataList << Data(value);
    }

    return dataList;
}


DataList ThemeWidget::getdata_air()
{
    QList<QPointF> dataList;
    qreal yValue(0);

    QFile file("C:\\Users\\mirth\\Desktop\\data_air.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))   // 文件打开失败
       { std::cerr<<"Chart data file could not open";
        return dataList;
    }

    while (!file.atEnd())
    {
        QString line = file.readLine();
        QStringList strList = line.split(" ", Qt::SkipEmptyParts);  // 以空格拆分字符串

        double x =strList.at(0).toDouble();
         yValue =strList.at(1).toDouble();

//        std::cout<<"x is "<<x<<" and y is "<<yValue<<std::endl;
        QPointF value(x,yValue);
        dataList << Data(value);
    }

    return dataList;
}

QChart *ThemeWidget::createSplineChart_of_co2()
{
    QChart* chart = m_ui->co2View->chart();
    chart->setTitle("CO2 chart");
    QString name("Series ");
    int nameIndex = 0;

    //std::cout<<"the chart funciont:"<<std::endl;
    DataList list = getdata_co2();   //Acruqire data ,type :QList<QPointF>

    QSplineSeries *series = new QSplineSeries(chart);
    qreal max_y1,max_y2,max_x,max_y;
    int flag =0;
    for (const Data &data : list)
    {
        if(flag==0)  // Acquire the upper bound of y
        {
            max_x =data.x();
            max_y1 = data.y();
            //std::cout<<"max_x is "<<max_x<<" and max_y is "<<max_y<<std::endl;
        }
        if(flag == list.size()-1)  // Acquire the upper bound of x
        {
            max_x = data.x();
            max_y2 = data.y();
            //std::cout<<"max_x is "<<max_x<<" and max_y is "<<max_y<<std::endl;
        }
        ++flag;
    }
    max_y  =max_y1 > max_y2 ? max_y1:max_y2;
    series->append(list);
    series->setName(name + QString::number(nameIndex));
    nameIndex++;
    chart->addSeries(series);



    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, max_x);
    chart->axes(Qt::Horizontal).first()->setTitleText("time(secs)");//标题
    chart->axes(Qt::Vertical).first()->setRange(0, max_y);
    chart->axes(Qt::Vertical).first()->setTitleText("co2(m^3");//标题

    // Add space to label to add space between labels and axis
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.5f  ");   // number of numbers preserved behind point
    return chart;
}


QChart *ThemeWidget::createSplineChart_of_temperature(){
    QChart* chart = m_ui->temperatureView_2->chart();
    chart->setTitle("Temperature chart");
    QString name("Series ");
    int nameIndex = 0;

    //std::cout<<"the chart funciont:"<<std::endl;
    DataList list = getdata_Temperature();   //Acruqire data ,type :QList<QPointF>
    QSplineSeries *series = new QSplineSeries(chart);
    qreal max_y1,max_y2,max_x,max_y;
    int flag =0;
    for (const Data &data : list)
    {
        if(flag==0)  // Acquire the upper bound of y
        {
            max_x =data.x();
            max_y1 = data.y();
            //std::cout<<"max_x is "<<max_x<<" and max_y is "<<max_y<<std::endl;
        }
        if(flag == list.size()-1)  // Acquire the upper bound of x
        {
            max_x = data.x();
            max_y2 = data.y();
            //std::cout<<"max_x is "<<max_x<<" and max_y is "<<max_y<<std::endl;
        }
        ++flag;
    }
    max_y  =max_y1 > max_y2 ? max_y1:max_y2;
    series->append(list);
    series->setName(name + QString::number(nameIndex));
    nameIndex++;
    chart->addSeries(series);



    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, max_x);
    chart->axes(Qt::Horizontal).first()->setTitleText("time(secs)");//标题
    chart->axes(Qt::Vertical).first()->setRange(-10, 40);
    chart->axes(Qt::Vertical).first()->setTitleText("t(℃");//标题

    // Add space to label to add space between labels and axis
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.5f  ");   // number of numbers preserved behind point
    axisY->setMinorTickCount(4);
    return chart;



}


QChart *ThemeWidget::createSplineChart_of_humidity(){
    QChart* chart = m_ui->humidityView_3->chart();
    chart->setTitle("Humidity chart");
    QString name("Series ");
    int nameIndex = 0;

    //std::cout<<"the chart funciont:"<<std::endl;
    DataList list = getdata_Humidity();   //Acruqire data ,type :QList<QPointF>
    QSplineSeries *series = new QSplineSeries(chart);
    qreal max_y1,max_y2,max_x,max_y;
    int flag =0;
    for (const Data &data : list)
    {
        if(flag==0)  // Acquire the upper bound of y
        {
            max_x =data.x();
            max_y1 = data.y();
            //std::cout<<"max_x is "<<max_x<<" and max_y is "<<max_y<<std::endl;
        }
        if(flag == list.size()-1)  // Acquire the upper bound of x
        {
            max_x = data.x();
            max_y2 = data.y();
            //std::cout<<"max_x is "<<max_x<<" and max_y is "<<max_y<<std::endl;
        }
        ++flag;
    }
    max_y  =max_y1 > max_y2 ? max_y1:max_y2;
    series->append(list);
    series->setName(name + QString::number(nameIndex));
    nameIndex++;
    chart->addSeries(series);



    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, max_x);
    chart->axes(Qt::Horizontal).first()->setTitleText("time(secs)");//标题
    chart->axes(Qt::Vertical).first()->setRange(0, 1);
    chart->axes(Qt::Vertical).first()->setTitleText("Relative_Humidity");//标题

    // Add space to label to add space between labels and axis
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.5f  ");   // number of numbers preserved behind point
    return chart;
}




QChart *ThemeWidget::createSplineChart_of_air()
{

    QChart* chart = m_ui->airView->chart();
    chart->setTitle("Air Temperature chart");
    QString name("Series ");
    int nameIndex = 0;

    //std::cout<<"the chart funciont:"<<std::endl;
    DataList list = getdata_air();   //Acruqire data ,type :QList<QPointF>
    QSplineSeries *series = new QSplineSeries(chart);
    qreal max_y1,max_y2,max_x,max_y;
    int flag =0;
    for (const Data &data : list)
    {
        if(flag==0)  // Acquire the upper bound of y
        {
            max_x =data.x();
            max_y1 = data.y();
            //std::cout<<"max_x is "<<max_x<<" and max_y is "<<max_y<<std::endl;
        }
        if(flag == list.size()-1)  // Acquire the upper bound of x
        {
            max_x = data.x();
            max_y2 = data.y();
            //std::cout<<"max_x is "<<max_x<<" and max_y is "<<max_y<<std::endl;
        }
        ++flag;
    }
    max_y  =max_y1 > max_y2 ? max_y1:max_y2;
    series->append(list);
    series->setName(name + QString::number(nameIndex));
    nameIndex++;
    chart->addSeries(series);



    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, max_x);
    chart->axes(Qt::Horizontal).first()->setTitleText("time(secs)");//标题
    chart->axes(Qt::Vertical).first()->setRange(-10, 40);  //max_y
    chart->axes(Qt::Vertical).first()->setTitleText("t(℃");//标题

    // Add space to label to add space between labels and axis
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.5f  ");   // number of numbers preserved behind point
    axisY->setMinorTickCount(4); //每个单位之间绘制了多少虚网线
    return chart;

}

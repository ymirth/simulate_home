
#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include<QFile>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include<iostream>
QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class Ui_ThemeWidgetForm;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPointF Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = 0);
    ~ThemeWidget();

private Q_SLOTS:
    //void updateUI();

private:

    void connectSignals();
    DataList getdata_Temperature();
    DataList getdata_co2();
    DataList getdata_Humidity();
    DataList getdata_air();
    QChart *createSplineChart_of_co2() ;
    QChart *createSplineChart_of_temperature() ;
    QChart *createSplineChart_of_humidity();
    QChart *createSplineChart_of_air();

private:
    QList<QChartView *> m_charts;
    Ui_ThemeWidgetForm *m_ui;
};

#endif /* THEMEWIDGET_H */




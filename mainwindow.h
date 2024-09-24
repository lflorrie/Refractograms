#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QChart>
#include <QtDataVisualization>
#include "refrlogic1.h"
#include <QXYSeries>
#include <QSplineSeries>
#include <QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//struct RefrCharts1 {
//    QChart chart;

//};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void plot2DPlotRefr1();

    int a;
private slots:
    void on_pushButton_clicked();
private:
    RefrLogicData get_value_from_input();
private:
    RefrLogic1 refr1;
    QChart *chart1;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

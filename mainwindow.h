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

private slots:
    void on_pushButton_clicked();
private:
	void plot2Dfunc_t(QChartView *chartView, QChart *chart);
	void plot2Dfunc_n(QChartView *chartView, QChart *chart);
	std::vector<QPointF> plot2Dfunc_refr(QChartView *chartView, QChart *chart, const std::vector<double> &z_array);
    RefrLogicData get_value_from_input();
private:
	// Plot for func_t
    RefrLogic1 refr1;
    QChart *chart1;
	QChart *chart2;

	QFont m_font;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

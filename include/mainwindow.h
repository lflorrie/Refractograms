#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QChart>
#include <QChartView>
#include <QtDataVisualization>
#include <QXYSeries>
#include <QSplineSeries>

#include "refrlogic1.h"
#include "refrcharts.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	void initRefr1();
    void plot2DPlotRefr1();

private slots:
    void on_pushButton_clicked();
private:
	void plot2Dfunc_t(int index);
	void plot2Dfunc_n(int index);
	std::vector<QPointF> plot2Dfunc_refr(QChartView *chartView, QChart *chart, const std::vector<double> &z_array);
	RefrLogicData getValuesFromInput();
	void setAxis2D();
private:
    RefrLogic1 refr1;

	RefrCharts		m_charts;
	QFont			m_font;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

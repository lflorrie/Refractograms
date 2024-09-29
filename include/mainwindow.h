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
private slots:
    void on_pushButton_clicked();
	void on_actionSaveAll();
private:
	RefrLogicData getValuesFromInput();
private:
	RefrCharts		*m_charts;
	QFont			m_font;
    Ui::MainWindow *ui;
	QThread thread;
};
#endif // MAINWINDOW_H

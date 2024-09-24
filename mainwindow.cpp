#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "refrlogic1.h"
#include <QtDebug>
#include "chartview.h"
#include "scattergraph.h"
#include "surfacegraph.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "OK";


    const QSize screenSize = ui->tabWidget->screen()->size();
    const QSize minimumGraphSize{screenSize.width() / 2, qRound(screenSize.height() / 1.75)};

    // Create scatter graph
    ScatterGraph scatter;
    // Create surface graph
    SurfaceGraph surface;

    // Add scatter widget
    if (!scatter.initialize(minimumGraphSize, screenSize)
        || !surface.initialize(minimumGraphSize, screenSize)) {
        QMessageBox::warning(nullptr, "Graph Gallery", "Couldn't initialize the OpenGL context.");
//        return -1;
    }
    ui->tabWidget->addTab(scatter.scatterWidget(), "Scatter Graph");


    // Add surface widget
//     ui->tabWidget->addTab(surface.surfaceWidget(), "Surface Graph");


    plot2DPlotRefr1();
}

MainWindow::~MainWindow()
{
    delete ui;
}
#include <QValueAxis>
void MainWindow::plot2DPlotRefr1() {
    // TAB 1
    auto dataPlot1 = refr1.make2DPlot([&](double x) { return refr1.func_t(x);} , refr1.getR(), refr1.getR() + 2, 100);
//    auto dataPlot1 = refr1.makePlotFuncT(refr1.getR(), refr1.getR() + 2, 100);

    QSplineSeries *series = new QSplineSeries;
    series->setName("func t");
    for (auto i : dataPlot1) {
        series->append(i);
    }
    QChart *chart = new QChart();
    this->chart1 = chart;
    chart->addSeries(series);
    chart->createDefaultAxes();

	ChartView *chartView = new ChartView(chart);
	chart->setTitle("Распределение температуры");
	// Grid settings
	QValueAxis *axisX = static_cast<QValueAxis *>(*chart->axes().begin());
	QValueAxis *axisY = static_cast<QValueAxis *>(*chart->axes(Qt::Vertical).begin());
	axisX->setTickType(QValueAxis::TicksDynamic);
	axisX->setTickInterval(0.5);


	// chart->addAxis(axisY, Qt::AlignLeft);
	chartView->setRenderHint(QPainter::Antialiasing);
	chart->legend()->setAlignment(Qt::AlignBottom);

//    ui->tabWidget->addTab(chartView, "2d plot t");


//    auto dataPlot2 = refr1.makePlotFuncN(refr1.getR(), refr1.getR() + 2, 100);
	auto dataPlot2 = refr1.make2DPlot([&](double x) { return refr1.func_n(x);} , refr1.getR(), refr1.getR() + 2, 100);
	QSplineSeries *series2 = new QSplineSeries;
	series2->setName("func n");
	for (auto i : dataPlot2) {
        series2->append(i);
    }
    QChart *chart2 = new QChart();
	chart2->setTitle("Распределение показателя преломления");
    chart2->legend()->setAlignment(Qt::AlignBottom);
    chart2->addSeries(series2);
    chart2->createDefaultAxes();

    QChartView *chartView2 = new QChartView(chart2);

    chartView2->setRenderHint(QPainter::Antialiasing);
//    ui->tabWidget->addTab(chartView2, "2d plot n");


    auto layoutTab1 = ui->tab->layout();
    layoutTab1->addWidget(chartView);
    layoutTab1->addWidget(chartView2);

    // TAB 2


    auto dataPlot3 = refr1.make2DPlot([&](double x) { return refr1.func_n(x);} , refr1.getR(), refr1.getR() + 2, 100);

    QSplineSeries *series3 = new QSplineSeries;
    series3->setName("func n");
    for (auto i : dataPlot3) {
        series3->append(i);
    }
    QChart *chart3 = new QChart();
    chart3->legend()->setAlignment(Qt::AlignBottom);
    chart3->addSeries(series3);
    chart3->createDefaultAxes();

    QChartView *chartView3 = new QChartView(chart3);

    chartView3->setRenderHint(QPainter::Antialiasing);

    auto layoutTab2 = ui->tab_2->layout();

    layoutTab2->addWidget(chartView3);

	QFont font;
	font.setBold(1);
	font.setPixelSize(20);
	chart2->setTitleFont(font);
	chart->setTitleFont(font);

	axisX->setLabelsEditable(1);
	axisX->setLabelsVisible(1);
	axisX->setTitleText("x, мм");
	axisY->setTitleText("t, °С");
}


void MainWindow::on_pushButton_clicked()
{
	qDebug() << "Update fields\n";
    refr1.set_values(get_value_from_input());
//    plot2DPlotRefr1();
    auto dataPlot1 = refr1.make2DPlot([&](double x) { return refr1.func_t(x);} , refr1.getR(), refr1.getR() + 2, 100);
    //    auto dataPlot1 = refr1.makePlotFuncT(refr1.getR(), refr1.getR() + 2, 100);

    QSplineSeries *series = new QSplineSeries;
    series->setName("func t");
    for (auto i : dataPlot1) {
        series->append(i);
    }
    this->chart1->removeAllSeries();
    this->chart1->addSeries(series);
}

RefrLogicData MainWindow::get_value_from_input()
{
    RefrLogicData data;
    data.R = ui->lineEdit->text().toDouble();
    data.a = ui->lineEdit_2->text().toDouble();
    data.z0 = ui->lineEdit_3->text().toDouble();
    data.deltaR = ui->lineEdit_4->text().toDouble();
    data.z1 = ui->lineEdit_5->text().toDouble();
    data.T0 = ui->lineEdit_6->text().toDouble();
    data.deltaT = ui->lineEdit_7->text().toDouble();
    data.x0 = ui->lineEdit_8->text().toDouble();
    return data;
}


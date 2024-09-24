#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "refrlogic1.h"
#include <QtDebug>
#include "chartview.h"
#include "scattergraph.h"
#include "surfacegraph.h"
#include <QMessageBox>
#include <QValueAxis>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	m_font.setBold(1);
	m_font.setPixelSize(20);

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
	qInfo() << "Main window created";
}

MainWindow::~MainWindow()
{
    delete ui;
	qInfo() << "Main window deleted";
}

void MainWindow::plot2DPlotRefr1() {
    // TAB 1
	chart1 = new QChart;
	chart2 = new QChart;

	QChartView *chartView = new QChartView(chart1);
	QChartView *chartView2 = new QChartView(chart2);

	plot2Dfunc_t(chartView, chart1);
	plot2Dfunc_n(chartView2, chart2);

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
}


void MainWindow::on_pushButton_clicked()
{
	qInfo() << "Button clicked. Updating fields for func_t and func_n.";
    refr1.set_values(get_value_from_input());
//    plot2DPlotRefr1();
    auto dataPlot1 = refr1.make2DPlot([&](double x) { return refr1.func_t(x);} , refr1.getR(), refr1.getR() + 2, 100);
    //    auto dataPlot1 = refr1.makePlotFuncT(refr1.getR(), refr1.getR() + 2, 100);
	auto dataPlot2 = refr1.make2DPlot([&](double x) { return refr1.func_n(x);}, refr1.getR(), refr1.getR() + 2, 100);

	{
		QSplineSeries *series = new QSplineSeries;
		series->setName("func t");
		for (auto i : dataPlot1) {
			series->append(i);
		}
		this->chart1->removeAllSeries();
		this->chart1->addSeries(series);
	}

	{
		QSplineSeries *series = new QSplineSeries;
		series->setName("func n");
		for (auto i : dataPlot1) {
			series->append(i);
		}
		this->chart2->removeAllSeries();
		this->chart2->addSeries(series);
	}
}

void MainWindow::plot2Dfunc_t(QChartView *chartView, QChart *chart)
{
	if (!chartView || !chart)
		return;

	auto dataPlot1 = refr1.make2DPlot([&](double x) { return refr1.func_t(x);} , refr1.getR(), refr1.getR() + 2, 100);
	//    auto dataPlot1 = refr1.makePlotFuncT(refr1.getR(), refr1.getR() + 2, 100);

	QSplineSeries *series = new QSplineSeries;
	series->setName("func t");
	for (auto i : dataPlot1) {
		series->append(i);
	}

	chart->addSeries(series);
	chart->createDefaultAxes();
	chart->setTitle("Распределение температуры");

	// Grid settings
	QValueAxis *axisX = static_cast<QValueAxis *>(*chart->axes().begin());
	QValueAxis *axisY = static_cast<QValueAxis *>(*chart->axes(Qt::Vertical).begin());
	axisX->setTickType(QValueAxis::TicksDynamic);
	axisX->setTickInterval(0.5);


	// chart->addAxis(axisY, Qt::AlignLeft);
	chartView->setRenderHint(QPainter::Antialiasing);
	chart->legend()->setAlignment(Qt::AlignBottom);

	chart->setTitleFont(m_font);

	axisX->setLabelsEditable(1);
	axisX->setLabelsVisible(1);
	axisX->setTitleText("x, мм");
	axisY->setTitleText("t, °С");
}

void MainWindow::plot2Dfunc_n(QChartView *chartView, QChart *chart)
{
	if (!chartView || !chart)
		return;
	auto dataPlot2 = refr1.make2DPlot([&](double x) { return refr1.func_n(x);} , refr1.getR(), refr1.getR() + 2, 100);
	QSplineSeries *series2 = new QSplineSeries;
	series2->setName("func n");
	for (auto i : dataPlot2) {
		series2->append(i);
	}
	chart->setTitle("Распределение показателя преломления");
	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->addSeries(series2);
	chart->createDefaultAxes();

	chartView->setRenderHint(QPainter::Antialiasing);

	chart->setTitleFont(m_font);

	// Grid settings
	QValueAxis *axisX = static_cast<QValueAxis *>(*chart->axes().begin());
	QValueAxis *axisY = static_cast<QValueAxis *>(*chart->axes(Qt::Vertical).begin());
	axisX->setTickType(QValueAxis::TicksDynamic);
	axisX->setTickInterval(0.5);

	axisX->setLabelsEditable(1);
	axisX->setLabelsVisible(1);
	qWarning() << "Wrong title for axis.";
	axisX->setTitleText("x, мм");
	axisY->setTitleText("t, °С");
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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "refrlogic1.h"
#include <QtDebug>
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
	qInfo() << "TAB1";
	chart1 = new QChart;
	QChartView *chartView = new QChartView(chart1);
	chartView->setRubberBand(QChartView::RectangleRubberBand);
	plot2Dfunc_t(chartView, chart1);
	auto layoutTab1 = ui->tab->layout();
	layoutTab1->addWidget(chartView);

	// TAB 2
	qInfo() << "TAB2";
	chart2 = new QChart;
	QChartView *chartView2 = new QChartView(chart2);
	chartView2->setRubberBand(QChartView::RectangleRubberBand);

	plot2Dfunc_n(chartView2, chart2);
	auto layoutTab2 = ui->tab_2->layout();
	layoutTab2->addWidget(chartView2);


	// TAB 3
	qInfo() << "TAB3";

	QChart *chart3 = new QChart();
	QChart *chart3_2 = new QChart();
	chart3->setTitle("Рефракционная картина");
	chart3_2->setTitle("Рефракционная картина");

	QChartView *chartView3 = new QChartView(chart3);
	QChartView *chartView3_2 = new QChartView(chart3_2);
	chartView3->setRubberBand(QChartView::RectangleRubberBand);
	chartView3_2->setRubberBand(QChartView::RectangleRubberBand);

	plot2Dfunc_refr(chartView3, chart3, {25});
	qInfo() << "TAB3_2";
	std::vector<double> z_array;
	double n = 9;
	for (int i = 0; i < (int)n; ++i)
	{
		z_array.push_back(25 + i * 25);
	}
	plot2Dfunc_refr(chartView3_2, chart3_2, z_array);

	auto layoutTab3 = ui->tab_3->layout();

	layoutTab3->addWidget(chartView3);
	layoutTab3->addWidget(chartView3_2);

	// TAB 4
/*
* for i in range(0, 10):
	for j in range(0, 100):
		z[i * 100 + j] = 20.0 + i * (440.0 - 20.0) / 10.0;
*/



	qInfo() << "plot2DPlotRefr1 end";
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

	auto dataPlot1 = refr1.make2DPlot([&](double x) { return refr1.func_t(x);} , refr1.getR(), refr1.getR() + 10, 100);
	//    auto dataPlot1 = refr1.makePlotFuncT(refr1.getR(), refr1.getR() + 2, 100);

	QSplineSeries *series = new QSplineSeries;
	series->setName("func t");
	for (auto i : dataPlot1) {
		series->append(i);
	}

	chart->addSeries(series);
	chart->createDefaultAxes();
	chart->setTitle("Зависимость температуры вдоль радиальной координаты");

	// Grid settings
	QValueAxis *axisX = static_cast<QValueAxis *>(chart->axes().at(0));
	QValueAxis *axisY = static_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
	axisX->setTickType(QValueAxis::TicksDynamic);
	axisX->setTickInterval(0.5);


	// chart->addAxis(axisY, Qt::AlignLeft);
	chartView->setRenderHint(QPainter::Antialiasing);
	chart->legend()->setAlignment(Qt::AlignBottom);

	chart->setTitleFont(m_font);

	axisX->setLabelsEditable(1);
	axisX->setLabelsVisible(1);
	axisX->setTitleText("x, мм");
	axisY->setTitleText("T, °С");
}

void MainWindow::plot2Dfunc_n(QChartView *chartView, QChart *chart)
{
	if (!chartView || !chart)
		return;
	auto dataPlot2 = refr1.make2DPlot([&](double x) { return refr1.func_n(x);} , refr1.getR(), refr1.getR() + 10, 100);
	QSplineSeries *series2 = new QSplineSeries;
	series2->setName("func n");
	for (auto i : dataPlot2) {
		series2->append(i);
	}
	chart->setTitle("Зависимость показателя преломления вдоль радиальной координаты");
	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->addSeries(series2);
	chart->createDefaultAxes();

	chartView->setRenderHint(QPainter::Antialiasing);

	chart->setTitleFont(m_font);

	// Grid settings
	QValueAxis *axisX = static_cast<QValueAxis *>(chart->axes().at(0));
	QValueAxis *axisY = static_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
	axisX->setTickType(QValueAxis::TicksDynamic);
	axisX->setTickInterval(0.5);

	axisX->setLabelsEditable(1);
	axisX->setLabelsVisible(1);
	axisX->setTitleText("r, мм");
	axisY->setTitleText("n");
}
#if 0
void MainWindow::plot2Dfunc_refr(QChartView *chartView, QChart *chart, const std::vector<double> &z_array)
{
	std::vector<QPointF> dataPlot3(100, {0.f, 0.f}); // TODO: optimization reserve n
	double fi_min = -M_PI_4;
	double fi_max = M_PI_4;
	int n = 100;
	double fi_step = (fi_max - fi_min) / n;

	QPointF point;

	for (auto zi : z_array)
	{
		double fi = fi_min;
		for (int i = 0; i < n; ++i)
		{
			qDebug() << "refr: func_x start";
			point.setY(refr1.func_x(fi, 20, zi)); // TODO: Replace constants on variables
			point.setX(refr1.func_y(fi, 20, zi));
			qDebug() << "refr: func_y end";
			dataPlot3[i] = point;
			fi += fi_step;
		}
	}

	QSplineSeries *series3 = new QSplineSeries;
	for (auto i : dataPlot3) {
		series3->append(i);
	}


	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->addSeries(series3);
	chart->createDefaultAxes();

	chartView->setRenderHint(QPainter::Antialiasing);
}
#endif
struct thread_{
	std::thread t;
	std::mutex l;
};
void async_calculate()
{

}
std::vector<QPointF> MainWindow::plot2Dfunc_refr(QChartView *chartView, QChart *chart, const std::vector<double> &z_array)
{
	double fi_min = -M_PI_4;
	double fi_max = M_PI_4;
	int n = 100;
	std::vector<QPointF> dataPlot3(n * z_array.size(), {0.f, 0.f});
	double fi_step = (fi_max - fi_min) / n;
	int threads_count = std::thread::hardware_concurrency();
	std::vector<thread_> threads(threads_count);
	qInfo() << "Count of threads: " << threads_count;
	// QPointF point;
	for (size_t zi = 0; zi < z_array.size(); ++zi)
	{
		double fi = fi_min;
		size_t offset = zi * n;
		for (size_t i = offset; i < n + offset; ++i)
		{
			size_t t_index = 0;
			while (1)
			{
				if (t_index == threads_count)
					t_index = 0;
				if (threads[t_index].l.try_lock())
				{
					if (threads[t_index].t.joinable())
					{
						threads[t_index].t.join();
					}
					// qDebug() << "Worker start" << t_index;
					threads[t_index].t = std::thread([&](double fi, double zi, size_t i, size_t t_index){
						QPointF point;
						point.setY(refr1.func_x(fi, 20, zi)); // TODO: Replace constants on variables
						point.setX(refr1.func_y(fi, 20, zi));
						dataPlot3[i] = point;
						// qDebug() << "Worker end" << t_index;
						threads[t_index].l.unlock();
					}, fi, z_array[zi], i, t_index);
					break;
				}
				t_index++;
			}
			fi += fi_step;
		}
	}
	for (int i = 0; i < threads_count; ++i)
	{
		if (threads[i].t.joinable())
		{
			threads[i].t.join();
			threads[i].l.unlock();
		}
	}
	for (int zi = 0; zi < z_array.size(); ++zi)
	{
		QSplineSeries *series3 = new QSplineSeries;
		size_t offset = zi * n;
		for (int i = offset; i < dataPlot3.size() / z_array.size() + offset; ++i) {
			if (dataPlot3[i].x() != dataPlot3[i].x() || dataPlot3[i].y() != dataPlot3[i].y()) {
				qDebug() << "NAN IGNORING";
				continue;
			}
			series3->append(dataPlot3[i]);
		}
		series3->setColor(QColor::fromRgb(0,0, zi * 20 + 20));
		auto pen = series3->pen();
		pen.setWidth(2);
		series3->setPen(pen);
		chart->addSeries(series3);
	}


	chart->legend()->setAlignment(Qt::AlignBottom);
	// chart->addSeries(series3);
	chart->createDefaultAxes();

	chartView->setRenderHint(QPainter::Antialiasing);

	chart->setTitleFont(m_font);

	// Grid settings
	QValueAxis *axisX = static_cast<QValueAxis *>(chart->axes().at(0));
	QValueAxis *axisY = static_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
	axisX->setTickType(QValueAxis::TicksDynamic);
	axisX->setTickInterval(20);

	axisX->setLabelsEditable(1);
	axisX->setLabelsVisible(1);
	axisX->setTitleText("y, мм");
	axisY->setTitleText("x, мм");
	return dataPlot3;
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


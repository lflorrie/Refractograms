#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "refrlogic1.h"
#include <QtDebug>
#include <QMessageBox>
#include <QValueAxis>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	m_font.setBold(1);
	m_font.setPixelSize(20);

	initRefr1();
	// plot2DPlotRefr1();
	qInfo() << "Main window created";
}

MainWindow::~MainWindow()
{
    delete ui;
	qInfo() << "Main window deleted";
}

void MainWindow::initRefr1() {
	for (int i = 0; i < m_charts.TAB_MAX; ++i)
	{
		m_charts.charts[i] = new QChart;
		m_charts.chartViews[i] = new QChartView(m_charts.charts[i]);
		m_charts.chartViews[i]->setRubberBand(QChartView::RectangleRubberBand);
		m_charts.chartViews[i]->setRenderHint(QPainter::Antialiasing);
		m_charts.charts[i]->legend()->setAlignment(Qt::AlignBottom);
		m_charts.charts[i]->setTitleFont(m_font);
		m_charts.charts[i]->setTitle(m_charts.labels[i].title);
	}
	ui->tab->layout()->addWidget(m_charts.chartViews[m_charts.TAB_1]);
	ui->tab_2->layout()->addWidget(m_charts.chartViews[m_charts.TAB_2]);
	ui->tab_3->layout()->addWidget(m_charts.chartViews[m_charts.TAB_3_1]);
	ui->tab_3->layout()->addWidget(m_charts.chartViews[m_charts.TAB_3_2]);

	m_charts.scatter3d = new Q3DScatter;
	QWidget *container = QWidget::createWindowContainer(m_charts.scatter3d);
	ui->tab_4->layout()->addWidget(container);

	m_charts.scatter3d->setAspectRatio(1.0); // TODO: make settings ?
	m_charts.scatter3d->setHorizontalAspectRatio(1.0);
}

void MainWindow::plot2DPlotRefr1() {
	qInfo() << "TAB1";
	plot2Dfunc_t(m_charts.TAB_1);

	qInfo() << "TAB2";
	plot2Dfunc_n(m_charts.TAB_2);

	// TODO: Need refactoring. Remove const values.
	qInfo() << "TAB3_1";
	plot2Dfunc_refr(m_charts.chartViews[m_charts.TAB_3_1], m_charts.charts[m_charts.TAB_3_1], {25});

	// TODO: Need refactoring. Remove const values.
	qInfo() << "TAB3_2";
	std::vector<double> z_array;
	double n = 3;
	for (int i = 0; i < (int)n; ++i)
	{
		z_array.push_back(25 + i * 25);
	}
	auto data_ = plot2Dfunc_refr(m_charts.chartViews[m_charts.TAB_3_2], m_charts.charts[m_charts.TAB_3_2], z_array);

	// TAB 4
	QScatterDataArray dataArray;
	dataArray.reserve(1000);

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			if (data_[j + i * 100].x() != data_[j + i * 100].x() ||
				data_[j + i * 100].y() != data_[j + i * 100].y())
				continue;
			dataArray.append(QScatterDataItem({(float)(25 + i * 25) , (float)data_[j + i * 100].y(), -(float)data_[j + i * 100].x()}));

		}
		qDebug() << i;
	}

	QScatter3DSeries *series = new QScatter3DSeries;
	series->dataProxy()->addItems(dataArray);

	// tab4 settings
	series->setItemSize(0.1);
	auto grad = QLinearGradient();
	grad.setColorAt(0, Qt::red);
	grad.setColorAt(1, Qt::blue);
	series->setBaseGradient(grad);
	series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
	series->setMeshSmooth(1);
	if (!m_charts.scatter3d->seriesList().empty())
		m_charts.scatter3d->removeSeries(m_charts.scatter3d->seriesList().at(0));
	m_charts.scatter3d->addSeries(series);

	qInfo() << "plot2DPlotRefr1 end";
	setAxis2D();
}


void MainWindow::on_pushButton_clicked()
{
	qInfo() << "Button clicked. Updating fields for func_t and func_n.";
	refr1.set_values(getValuesFromInput());
	plot2DPlotRefr1();
}

void MainWindow::setAxis2D() {
	for (int i = 0; i < m_charts.TAB_MAX; ++i)
	{
		m_charts.charts[i]->createDefaultAxes();
		QValueAxis *axisX = static_cast<QValueAxis *>(m_charts.charts[i]->axes().at(0));
		QValueAxis *axisY = static_cast<QValueAxis *>(m_charts.charts[i]->axes(Qt::Vertical).at(0));
		// axisX->setTickType(QValueAxis::TicksDynamic); // TODO: add to settings
		// axisX->setTickInterval(0.5); // TODO: add to settings
		axisX->setLabelsEditable(1);
		axisX->setLabelsVisible(1);
		axisX->setTitleText(m_charts.labels[i].axisXTitle);
		axisY->setTitleText(m_charts.labels[i].axisYTitle);
	}
}

void MainWindow::plot2Dfunc_t(int index)
{
	if (index < 0 || index >= m_charts.TAB_MAX)
		return;
	QSplineSeries *series = new QSplineSeries;

	std::vector<QPointF> dataPlot = refr1.make2DPlot([&](double x) { return refr1.func_t(x);}, refr1.getR(), refr1.getR() + 10, 100);

	m_charts.charts[index]->removeAllSeries();
	for (auto i : dataPlot) {
		series->append(i);
	}
	m_charts.charts[index]->addSeries(series);
}

void MainWindow::plot2Dfunc_n(int index)
{
	if (index < 0 || index >= m_charts.TAB_MAX)
		return;
	QSplineSeries *series = new QSplineSeries;

	std::vector<QPointF> dataPlot = refr1.make2DPlot([&](double x) { return refr1.func_n(x);} , refr1.getR(), refr1.getR() + 10, 100);

	m_charts.charts[index]->removeAllSeries();
	for (auto i : dataPlot) {
		series->append(i);
	}
	m_charts.charts[index]->addSeries(series);
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

std::vector<QPointF> MainWindow::plot2Dfunc_refr(QChartView *chartView, QChart *chart, const std::vector<double> &z_array)
{
	chart->removeAllSeries();
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
	return dataPlot3;
}

RefrLogicData MainWindow::getValuesFromInput()
{
    RefrLogicData data;
	data.R		= ui->lineEdit->text().toDouble();
	data.a		= ui->lineEdit_2->text().toDouble();
	data.z0		= ui->lineEdit_3->text().toDouble();
    data.deltaR = ui->lineEdit_4->text().toDouble();
	data.z1		= ui->lineEdit_5->text().toDouble();
	data.T0		= ui->lineEdit_6->text().toDouble();
    data.deltaT = ui->lineEdit_7->text().toDouble();
	data.x0		= ui->lineEdit_8->text().toDouble();
    return data;
}


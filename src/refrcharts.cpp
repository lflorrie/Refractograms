#include "refrcharts.h"
#include <QValueAxis>
#include <QLayout>
#include "chartview.h"

RefrCharts::RefrCharts()
{
	initRefr1();
}

RefrCharts::RefrCharts(const std::vector<QLayout *> layouts, QFont *font)
{
	m_layouts = layouts;
	m_font = *font;
	initRefr1();
	workerIsRunning = false;
}

void RefrCharts::initRefr1() {
	for (int i = 0; i < TAB_MAX; ++i)
	{
		charts[i] = new QChart;
		chartViews[i] = new ChartView(charts[i]);

		charts[i]->legend()->setAlignment(Qt::AlignBottom);
		charts[i]->setTitleFont(m_font);
		charts[i]->setTitle(labels[i].title);
	}
	if (m_layouts.size() >= 3)
	{
		m_layouts[0]->addWidget(chartViews[TAB_1]);
		m_layouts[1]->addWidget(chartViews[TAB_2]);
		m_layouts[2]->addWidget(chartViews[TAB_3_1]);
		m_layouts[2]->addWidget(chartViews[TAB_3_2]);
	}

	scatter3d = new Scatter3D;
	container = QWidget::createWindowContainer(scatter3d);
	if (m_layouts.size() >= 4)
		m_layouts[3]->layout()->addWidget(container);

	scatter3d->setAspectRatio(1.0); // TODO: make settings ?
	scatter3d->setHorizontalAspectRatio(1.0);
	// setAxis2D();
}

void RefrCharts::buildPlots(const RefrLogicData &values, Plots p)
{
	if (workerIsRunning)
	{
		qInfo() << "Wo3rker is running.";
		return;
	}
	workerIsRunning = 1;
	refrWorker = new RefrWorker;
	thread = new QThread;
	refrWorker->setValues(values);
	refrWorker->setTypeOfCalculation(p);
	refrWorker->moveToThread(thread);
	connect(thread, &QThread::started, refrWorker, &RefrWorker::process);
	connect(refrWorker, &RefrWorker::finished, this, &RefrCharts::onWorkerFinished);
	connect(refrWorker, &RefrWorker::finished, thread, &QThread::quit);
	connect(thread, &QThread::finished, refrWorker, &QObject::deleteLater);
	connect(thread, &QThread::finished, thread, &QObject::deleteLater);

	connect(refrWorker, &RefrWorker::finished, this, &RefrCharts::finished);
	connect(refrWorker, &RefrWorker::progressChanged, this, &RefrCharts::progressChanged);


	qInfo() << "Worker start.";
	thread->start();
}

void RefrCharts::setAxis2D() {
	QValueAxis *axisX;
	QValueAxis *axisY;

	for (int i = 0; i < TAB_MAX; ++i)
	{
		charts[i]->createDefaultAxes();
		if (charts[i]->axes().isEmpty())
			continue;
		axisX = static_cast<QValueAxis *>(charts[i]->axes().at(0));
		axisY = static_cast<QValueAxis *>(charts[i]->axes(Qt::Vertical).at(0));
		// axisX->setTickType(QValueAxis::TicksDynamic); // TODO: add to settings
		// axisX->setTickInterval(0.5); // TODO: add to settings
		axisX->setLabelsEditable(1);
		axisX->setLabelsVisible(1);

		axisX->setTitleText(labels[i].axisXTitle);
		axisY->setTitleText(labels[i].axisYTitle);
	}
}
// TODO: refactoring this function
void RefrCharts::onWorkerFinished(Plots type)
{
	auto data     = refrWorker->getData();
	auto refrData = refrWorker->getRefrData().getData();
	// append data
	std::vector<QSplineSeries *>splines[4];
	if (type == PLOT_3_1)
	{
		splines[TAB_3_1].push_back(new QSplineSeries());
		charts[TAB_3_1]->removeAllSeries();
		for (size_t zi = 0; zi < 1; ++zi) {
			for (int i = 0; i < refrData.z_settings.count_of_points; ++i) {
				if (data->dataPlot[TAB_3_1][i].x() != data->dataPlot[TAB_3_1][i].x() ||
					data->dataPlot[TAB_3_1][i].y() != data->dataPlot[TAB_3_1][i].y())
					continue;
				splines[TAB_3_1][zi]->append(data->dataPlot[TAB_3_1][i + zi * refrData.z_settings.count_of_points]);
			}
		}
		for (auto &spline : splines[TAB_3_1])
		{
			charts[TAB_3_1]->addSeries(spline);
			connect(spline, &QSplineSeries::clicked, chartViews[TAB_3_1], &ChartView::keepCallout);
			connect(spline, &QSplineSeries::hovered, chartViews[TAB_3_1], &ChartView::tooltip);
		}
		setAxis2D();
		workerIsRunning = false;
		return;
	}
	for (int i = 0 ; i < TAB_MAX - 1; ++i)
	{
		splines[i].push_back(new QSplineSeries());
	}
	for (int i = 0 ; i < refrData.z_settings.count; ++i)
	{
		splines[TAB_3_2].push_back(new QSplineSeries());
	}
	for (auto &i : data->dataPlot[TAB_1]) {
		splines[TAB_1].front()->append(i);
	}
	for (auto &i : data->dataPlot[TAB_2]) {
		splines[TAB_2].front()->append(i);
	}

	for (size_t zi = 0; zi < 1; ++zi) {
		for (int i = 0; i < refrData.z_settings.count_of_points; ++i) {
			if (data->dataPlot[TAB_3_1][i].x() != data->dataPlot[TAB_3_1][i].x() ||
				data->dataPlot[TAB_3_1][i].y() != data->dataPlot[TAB_3_1][i].y())
				continue;
			splines[TAB_3_1][zi]->append(data->dataPlot[TAB_3_1][i + zi * refrData.z_settings.count_of_points]);
		}
	}

	for (int zi = 0; zi < refrData.z_settings.count; ++zi) {
		for (int i = 0; i < refrData.z_settings.count_of_points; ++i) {
			if (data->dataPlot[TAB_3_2][i].x() != data->dataPlot[TAB_3_2][i].x() ||
				data->dataPlot[TAB_3_2][i].y() != data->dataPlot[TAB_3_2][i].y())
				continue;
			splines[TAB_3_2][zi]->append(data->dataPlot[TAB_3_2][i + zi * refrData.z_settings.count_of_points]);
		}
	}

	for (int i = 0; i < TAB_MAX; ++i)
	{
		charts[i]->removeAllSeries();
		for (auto &spline : splines[i])
		{
			charts[i]->addSeries(spline);
			connect(spline, &QSplineSeries::clicked, chartViews[i], &ChartView::keepCallout);
			connect(spline, &QSplineSeries::hovered, chartViews[i], &ChartView::tooltip);
		}
	}

	if (!scatter3d->seriesList().empty())
		scatter3d->removeSeries(scatter3d->seriesList().at(0));
	scatter3d->addSeries(data->scatter3d);

	setAxis2D();
	workerIsRunning = false;
}

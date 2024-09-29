#include "refrworker.h"
#include "refrcharts.h"

RefrWorker::RefrWorker()
{
	std::cout << "RefrWorker thread id: " << std::this_thread::get_id() << std::endl;
}

void RefrWorker::process()
{
	double n = 3;
	std::cout << "RefrWorker process thread id: " << std::this_thread::get_id() << std::endl;

	// clear splines
	// for (int i = 0 ; i < 3; ++i)
	// {
	// 	data.splines[i].push_back(new QSplineSeries());
	// }
	// for (int i = 0; i < n; ++i)
	// {
	// 	data.splines[RefrCharts::TAB_3_2].push_back(new QSplineSeries());
	// }
	data.scatter3d = new QScatter3DSeries;
	for (int i = 0; i < RefrCharts::TAB_MAX; ++i)
	{
		data.dataPlot[i].clear();
	}

	// calculation
	qInfo() << "TAB1";
	data.dataPlot[RefrCharts::TAB_1] = refr1.make2DPlot([&](double x) { return refr1.func_t(x);}, refr1.getR(), refr1.getR() + 10, 100);
	qInfo() << "TAB2";
	data.dataPlot[RefrCharts::TAB_2] = refr1.make2DPlot([&](double x) { return refr1.func_n(x);} , refr1.getR(), refr1.getR() + 10, 100);
	qInfo() << "TAB3_1";

	data.dataPlot[RefrCharts::TAB_3_1] = plot2Dfunc_refr({25});
	qInfo() << "TAB3_2";
	std::vector<double> z_array;
	for (int i = 0; i < (int)n; ++i)
	{
		z_array.push_back(25 + i * 25);
	}
	data.dataPlot[RefrCharts::TAB_3_2] =plot2Dfunc_refr(z_array);

	// TAB 4
	QScatterDataArray dataArray;
	dataArray.reserve(1000);

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			if (data.dataPlot[RefrCharts::TAB_3_2][j + i * 100].x() != data.dataPlot[RefrCharts::TAB_3_2][j + i * 100].x() ||
				data.dataPlot[RefrCharts::TAB_3_2][j + i * 100].y() != data.dataPlot[RefrCharts::TAB_3_2][j + i * 100].y())
				continue;
			dataArray.append(QScatterDataItem({(float)(25 + i * 25) , (float)data.dataPlot[RefrCharts::TAB_3_2][j + i * 100].y(), -(float)data.dataPlot[RefrCharts::TAB_3_2][j + i * 100].x()}));

		}
		qDebug() << i;
	}

	data.scatter3d->dataProxy()->addItems(dataArray);

	// tab4 settings
	data.scatter3d->setItemSize(0.1);
	auto grad = QLinearGradient();
	grad.setColorAt(0, Qt::red);
	grad.setColorAt(1, Qt::blue);
	data.scatter3d->setBaseGradient(grad);
	data.scatter3d->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
	data.scatter3d->setMeshSmooth(1);

	qInfo() << "plot2DPlotRefr1 end";
	emit finished();
}

RefrWorker::SeriesData *RefrWorker::getData()
{
	std::cout << "RefrWorker getData thread id: " << std::this_thread::get_id() << std::endl;
	return &data;
}

void RefrWorker::setValues(const RefrLogicData &values)
{
	refr1.set_values(values);
}

struct thread_{
	std::thread t;
	std::mutex l;
};

std::vector<QPointF> RefrWorker::plot2Dfunc_refr(const std::vector<double> &z_array)
{

	double fi_min = -M_PI_4;
	double fi_max = M_PI_4;
	int n = 100;
	double fi_step = (fi_max - fi_min) / n;
	size_t threads_count = std::thread::hardware_concurrency();
	std::vector<QPointF> dataPlot3(n * z_array.size(), {0.f, 0.f});
	std::vector<thread_> threads(threads_count);
	qInfo() << "Count of threads: " << threads_count;
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
	for (size_t i = 0; i < threads_count; ++i)
	{
		if (threads[i].t.joinable())
		{
			threads[i].t.join();
			threads[i].l.unlock();
		}
	}
	return dataPlot3;
}

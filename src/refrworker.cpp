#include "refrworker.h"
#include "refrcharts.h"

RefrWorker::RefrWorker()
{
}

void RefrWorker::process()
{
	emit progressChanged(0);
	int		count_z = refr1.getData().z_settings.count;
	double	R = refr1.getData().R;
	int		count_of_points = refr1.getData().z_settings.count_of_points;
	double  start = refr1.getData().z_settings.start;
	double  step = refr1.getData().z_settings.step;

	data.scatter3d = new QScatter3DSeries;
	for (int i = 0; i < RefrCharts::TAB_MAX; ++i)
	{
		data.dataPlot[i].clear();
	}

	// calculation
	qInfo() << "TAB1";
	data.dataPlot[RefrCharts::TAB_1] = refr1.make2DPlot([&](double x) { return refr1.func_t(x);}, R, R + 10, count_of_points);
	qInfo() << "TAB2";
	data.dataPlot[RefrCharts::TAB_2] = refr1.make2DPlot([&](double x) { return refr1.func_n(x);} , R, R + 10, count_of_points);
	qInfo() << "TAB3_1";
	emit progressChanged(5);
	data.dataPlot[RefrCharts::TAB_3_1] = plot2Dfunc_refr({refr1.getData().z_settings.current});
	emit progressChanged(10);
	qInfo() << "TAB3_2";
	std::vector<double> z_array;
	for (int i = 0; i < count_z; ++i)
	{
		z_array.push_back(start + i * step);
	}
	data.dataPlot[RefrCharts::TAB_3_2] = plot2Dfunc_refr(z_array);

	// TAB 4
	QScatterDataArray dataArray;
	dataArray.reserve(count_of_points * count_z);

	for (int i = 0; i < count_z; ++i)
	{
		for (int j = 0; j < count_of_points; ++j)
		{
			if (data.dataPlot[RefrCharts::TAB_3_2][j + i * count_of_points].x() != data.dataPlot[RefrCharts::TAB_3_2][j + i * count_of_points].x() ||
				data.dataPlot[RefrCharts::TAB_3_2][j + i * count_of_points].y() != data.dataPlot[RefrCharts::TAB_3_2][j + i * count_of_points].y())
				continue;
			dataArray.append(QScatterDataItem({(float)(start + i * step) , (float)data.dataPlot[RefrCharts::TAB_3_2][j + i * count_of_points].y(), -(float)data.dataPlot[RefrCharts::TAB_3_2][j + i * count_of_points].x()}));
		}
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

	qInfo() << "process end";
	emit finished();
}

RefrWorker::SeriesData *RefrWorker::getData()
{
	return &data;
}

RefrLogic1 RefrWorker::getRefrData() const
{
	return refr1;
}

void RefrWorker::setValues(const RefrLogicData &values)
{
	refr1.setData(values);
}

struct thread_{
	std::thread t;
	std::mutex  l;
};

std::vector<QPointF> RefrWorker::plot2Dfunc_refr(const std::vector<double> &z_array)
{

	double fi_min = -M_PI_4;
	double fi_max = M_PI_4;
	int n = refr1.getData().z_settings.count_of_points;
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
						point.setY(refr1.func_x(fi, refr1.getData().x0, zi));
						point.setX(refr1.func_y(fi, refr1.getData().x0, zi));
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
		emit progressChanged(10 + ((zi + 1) / (double)z_array.size() * 90));
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

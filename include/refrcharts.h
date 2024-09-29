#ifndef REFRCHARTS_H
#define REFRCHARTS_H
#include <QChart>
#include <QChartView>
#include <QtDataVisualization>
#include <QSplineSeries>
#include "refrworker.h"

struct RefrCharts : public QObject {
	Q_OBJECT
public:
	RefrCharts();
	RefrCharts(const std::vector<QLayout *> layouts, QFont *font);

	void initRefr1();
	void plot2DPlotRefr1(const RefrLogicData &values);
	void setAxis2D();
	void plot2Dfunc_t(int index);
	void plot2Dfunc_n(int index);
	// std::vector<QPointF> plot2Dfunc_refr(QChartView *chartView, QChart *chart, const std::vector<double> &z_array);
	enum {
		TAB_1,
		TAB_2,
		TAB_3_1,
		TAB_3_2,
		TAB_MAX
	};

	struct RefrChartsTextLables{
		QString title;
		QString axisXTitle;
		QString axisYTitle;
		QString seriesName;
	} labels[TAB_MAX] = {
		{"Зависимость температуры вдоль радиальной координаты", "x, мм", "T, °С", "func t" },
		{"Зависимость показателя преломления вдоль радиальной координаты","r, мм", "n", "func n"},
		{"Рефракционная картина","","",""},
		{"Рефракционная картина","","",""}
	};

	QWidget			*container;
	Q3DScatter		*scatter3d;
	QChart			*charts[TAB_MAX];
	QChartView		*chartViews[TAB_MAX];
	std::vector<QLayout *> m_layouts;
	QFont m_font;

	bool workerIsRunning;
	QThread			*thread;
	RefrWorker		*refrWorker;
public slots:
	void onWorkerFinished();
};

#endif // REFRCHARTS_H



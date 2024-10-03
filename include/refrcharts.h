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
	void setAxis2D();
	enum Plots {
		PLOT_1,
		PLOT_2,
		PLOT_3_1,
		PLOT_3_2,
		PLOT_4,
		ALL_PLOTS
	};
	enum {
		TAB_1,
		TAB_2,
		TAB_3_1,
		TAB_3_2,
		TAB_MAX
	};
	void buildPlots(const RefrLogicData &values, RefrCharts::Plots);

	struct RefrChartsTextLables{
		QString title;
		QString axisXTitle;
		QString axisYTitle;
		QString seriesName;
	} labels[TAB_MAX] = {
		{tr("Зависимость температуры вдоль радиальной координаты"), tr("x, мм"), tr("T, °С"), tr("func t")},
		{tr("Зависимость показателя преломления вдоль радиальной координаты"),tr("r, мм"), tr("n"), tr("func n")},
		{tr("Рефракционная картина"),tr(""),tr(""),tr("")},
		{tr("Рефракционная картина"),tr(""),tr(""),tr("")}
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
signals:
	void progressChanged(int value);
	void finished();
};

#endif // REFRCHARTS_H



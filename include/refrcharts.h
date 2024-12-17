#ifndef REFRCHARTS_H
#define REFRCHARTS_H
#include <QChart>
#include <QChartView>
#include <QtDataVisualization>
#include <QSplineSeries>
#include "refrworker.h"
#include "chartview.h"
#include "scatter3d.h"
#include "refrtypes.h"

struct RefrCharts : public QObject {
	Q_OBJECT
public:
	RefrCharts();
	RefrCharts(const std::vector<QLayout *> layouts, QFont *font);

	void initRefr1();
	void setAxis2D();
	void buildPlots(const RefrLogicData &values, Plots);

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
	Scatter3D		*scatter3d;

	QChart			*charts[TAB_MAX];
	ChartView		*chartViews[TAB_MAX];
	std::vector<QLayout *> m_layouts;
	QFont m_font;

	bool workerIsRunning;
	QThread			*thread;
	RefrWorker		*refrWorker;

public slots:
	void onWorkerFinished(Plots type);
signals:
	void progressChanged(int value);
	void finished();
};

#endif // REFRCHARTS_H



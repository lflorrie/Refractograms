#ifndef REFRCHARTS_H
#define REFRCHARTS_H
#include <QChart>
#include <QChartView>
#include <QtDataVisualization>
#include <QSplineSeries>

struct RefrCharts {
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

	Q3DScatter		*scatter3d;
	QChart			*charts[TAB_MAX];
	QChartView		*chartViews[TAB_MAX];
};

#endif // REFRCHARTS_H

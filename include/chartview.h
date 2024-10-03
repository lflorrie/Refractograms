#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>
#include "settingschartview.h"

class ChartView : public QChartView
{
	Q_OBJECT
public:
	ChartView();
	ChartView(QChart *chart) : QChartView(chart) {

	}
public slots:

	// QWidget interface
protected:
	virtual void mousePressEvent(QMouseEvent *event) override;
private:
	// SettingsChartView settingsDialogWindow;
};

#endif // CHARTVIEW_H

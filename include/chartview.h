#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>
#include "settingschartview.h"
#include <QValueAxis>
#include <QLabel>

class ChartView : public QChartView
{
	Q_OBJECT
public:
	ChartView();
	ChartView(QChart *chart) : QChartView(chart) {
		this->setMouseTracking(1);

		// connect(this, &ChartView::mouseMoveEvent, [=](QMouseEvent *event) {
			// tooltip->setVisible(false);
		// });
	}
	void saveContent(const QString &full_path);
	void exportContent(const QString &full_path);
public slots:
	// QWidget interface
protected:
	virtual void mousePressEvent(QMouseEvent *event) override;
private:
	// SettingsChartView settingsDialogWindow;

	// QWidget interface
protected:
	virtual void mouseMoveEvent(QMouseEvent *event) override;

	// QObject interface
public:
	virtual bool event(QEvent *event) override;

};

#endif // CHARTVIEW_H

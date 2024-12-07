#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>
#include "settingschartview.h"
#include <QValueAxis>
#include <QLabel>
#include "callout.h"

class ChartView : public QChartView
{
	Q_OBJECT
public:
	ChartView();
	ChartView(QChart *chart);
	void saveContent(const QString &full_path);
	void exportContent(const QString &full_path);
public slots:
	// QWidget interface
protected:
	virtual void mousePressEvent(QMouseEvent *event) override;
private:
	SettingsChartView *settingsDialogWindow;

	// QWidget interface
protected:
	virtual void mouseMoveEvent(QMouseEvent *event) override;

	// QObject interface
public:
	virtual bool event(QEvent *event) override;

public slots:
	void keepCallout();
	void tooltip(QPointF point, bool state);
private:
	QGraphicsSimpleTextItem *m_coordX;
	QGraphicsSimpleTextItem *m_coordY;
	Callout *m_tooltip;
	QList<Callout *> m_callouts;

	// QWidget interface
protected:
	virtual void resizeEvent(QResizeEvent *event) override;

	// QWidget interface
protected:
	virtual void wheelEvent(QWheelEvent *event) override;
};

#endif // CHARTVIEW_H

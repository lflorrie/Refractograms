#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>
#include "settingschartview.h"
#include <QValueAxis>
#include <QLabel>
#include "callout.h"
#include "settings.h"

class ChartView : public QChartView
{
	Q_OBJECT
public:
	ChartView();
	ChartView(QChart *chart);
	void saveContent(const QString &full_path);
	void exportContent(const QString &full_path);
    Settings &settings()
    {
        return m_settings;
    }
public slots:
    void keepCallout();
    void tooltip(QPointF point, bool state);
    void onSettingsUpdated(PlotSettings &p);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

private:
	QGraphicsSimpleTextItem *m_coordX;
	QGraphicsSimpleTextItem *m_coordY;
	Callout *m_tooltip;
    QList<Callout *> m_callouts;
    SettingsChartView *settingsDialogWindow;
    Settings m_settings;
};

#endif // CHARTVIEW_H

#include "chartview.h"
#include <QDebug>


ChartView::ChartView() {}

void ChartView::mousePressEvent(QMouseEvent *event)
{
	qDebug() << "ChartView: MousePressEvent";
	if (event->button() == Qt::RightButton)
	{
		qDebug() << "	ChartView: Right mouse click";
		settingsDialogWindow.open();
	}
}

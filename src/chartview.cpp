#include "chartview.h"
#include <QDebug>
#include <QMenu>

ChartView::ChartView() {}

void ChartView::mousePressEvent(QMouseEvent *event)
{
	qDebug() << "ChartView: MousePressEvent";
	// if (event->button() == Qt::RightButton)
	// {
	// 	qDebug() << "	ChartView: Right mouse click";
	// 	settingsDialogWindow.open();
	// }

	if (event->button() == Qt::RightButton) {
		QMenu menu;
		QAction* action1 = menu.addAction("Save");
		QAction* action2 = menu.addAction("Save data");
		QAction* action3 = menu.addAction("Preferences");

		QAction* selectedAction = menu.exec(event->globalPos());

		if (selectedAction == action1) {
			// Действие для Action 1
		} else if (selectedAction == action2) {
			// Действие для Action 2
		} else if (selectedAction == action3) {

		}
	} else {
		QChartView::mousePressEvent(event);
	}

}

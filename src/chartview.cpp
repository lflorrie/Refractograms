#include "chartview.h"
#include <QDebug>
#include <QMenu>
#include <QFileDialog>
#include <QSplineSeries>

ChartView::ChartView() {
	// this->setMouseTracking(1);
	// tooltip = new QGraphicsSimpleTextItem();
	// // tooltip->setPos(10, 10);
	// tooltip->setZValue(11); // Устанавливаем z-индекс, чтобы подсказка была поверх графика
	// tooltip->setVisible(false); // По умолчанию подсказка скрыта
}

void ChartView::saveContent(const QString &path)
{
	if (path.isEmpty())
		return;
	qDebug() << "Save content of chartview." << path;

	QSizeF sizeChart = this->chart()->size();
	QSize sizeView = this->size();
	QSize resolution = {1920, 1080};

	this->chart()->resize(resolution);
	this->resize(resolution);

	this->grab({QPoint(0, 0), resolution}).save(path);

	this->chart()->resize(sizeChart);
	this->resize(sizeView);
}

void ChartView::exportContent(const QString &path)
{
	if (path.isEmpty() || !this->chart()->series().size())
		return;
	qDebug() << "Export content. Seieses size:" << this->chart()->series().size();
	QFile file(path);
	file.open(QFile::ReadWrite);
	for (int i = 0; i < this->chart()->series().size(); ++i)
	{
		auto		  dataToSave = this->chart()->series().at(i);
		QSplineSeries *series = qobject_cast<QSplineSeries *>(dataToSave);

		if (!series)
			return;

		foreach (const QPointF &point, series->points()) {
			std::string line = QString::number(point.x()).toStdString() + " "
							   + QString::number(point.y()).toStdString() + "\n";
			file.write(line.c_str());
		}
		file.write("\n");
	}
	file.close();

}

void ChartView::mousePressEvent(QMouseEvent *event)
{
	qDebug() << "ChartView: MousePressEvent";
	// if (event->button() == Qt::RightButton)
	// {
	// 	qDebug() << "	ChartView: Right mouse click";
	// 	settingsDialogWindow.open();
	// }

	if (event->button() == Qt::MiddleButton) {
		QMenu menu;
		QAction* save = menu.addAction("Save");
		QAction* save_data = menu.addAction("Save data");
		QAction* preferences = menu.addAction("Preferences");

		QAction* selectedAction = menu.exec(event->globalPos());

		if (selectedAction == save) {
			QString fullPath = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(),"*.png");
			saveContent(fullPath);
		} else if (selectedAction == save_data) {
			QString fullPath = QFileDialog::getSaveFileName(this, tr("Export as..."), QString(),"*.txt");
			exportContent(fullPath);
		} else if (selectedAction == preferences) {
			// settingsDialogWindow.open();
		}
	}
	QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{

	QPointF pos = this->mapToScene(event->pos());
	try {
	// QValueAxis *axisX = static_cast<QValueAxis *>(this->chart()->axes().at(0));
	// QValueAxis *axisY = static_cast<QValueAxis *>(this->chart()->axes(Qt::Vertical).at(0));
		auto series = this->chart()->series();
		if (series.isEmpty())
			return;
		auto val = this->chart()->mapToValue(pos, series.at(0));

		qDebug() << event->type() <<  " X: " << val.x() << ", Y: " << val.y();
	} catch (...)
	{
		return;
	}

	// if (!axisX || !axisY)
	// return;
	// qreal xVal = chart->mapToValue(pos, axisX);
	// qreal yVal = chart->mapToValue(pos, axisY);
	// qreal xVal = pos.x();
	// qreal yVal = pos.y();
	// qDebug() << event->type() <<  " X: " << xVal << ", Y: " << yVal;
	QChartView::mouseMoveEvent(event);
}

bool ChartView::event(QEvent *event)
{
	if (event->type() == QEvent::Leave) {
		qDebug() << "MOUSE LEAVE";
		// tooltip->setVisible(0);
	}
	return QChartView::event(event);
}

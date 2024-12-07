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
    m_tooltip = 0;
}

ChartView::ChartView(QChart *chart) : QChartView(chart) {
	settingsDialogWindow = new SettingsChartView(this);
	this->setMouseTracking(true);
	this->setRenderHint(QPainter::Antialiasing);
	// this->setRubberBand(QChartView::RectangleRubberBand);


	m_coordX = new QGraphicsSimpleTextItem(this->chart());
	m_coordX->setPos(this->chart()->size().width()/2 - 50, this->chart()->size().height());
	m_coordX->setText("X: ");
	m_coordY = new QGraphicsSimpleTextItem(this->chart());
	m_coordY->setPos(this->chart()->size().width()/2 + 50, this->chart()->size().height());
	m_coordY->setText("Y: ");
    m_tooltip = 0;
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
			settingsDialogWindow->open();
		}
	}
	QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{

	// QPointF pos = this->mapToScene(event->pos());
	// try {
	// // QValueAxis *axisX = static_cast<QValueAxis *>(this->chart()->axes().at(0));
	// // QValueAxis *axisY = static_cast<QValueAxis *>(this->chart()->axes(Qt::Vertical).at(0));
	// 	auto series = this->chart()->series();
	// 	if (series.isEmpty())
	// 		return;
	// 	auto val = this->chart()->mapToValue(pos, series.at(0));

	// 	qDebug() << event->type() <<  " X: " << val.x() << ", Y: " << val.y();
	// } catch (...)
	// {
	// 	return;
	// }

	// if (!axisX || !axisY)
	// return;
	// qreal xVal = chart->mapToValue(pos, axisX);
	// qreal yVal = chart->mapToValue(pos, axisY);
	// qreal xVal = pos.x();
	// qreal yVal = pos.y();
	// qDebug() << event->type() <<  " X: " << xVal << ", Y: " << yVal;
	m_coordX->setText(QString("X: %1").arg(chart()->mapToValue(event->pos()).x()));
	m_coordY->setText(QString("Y: %1").arg(chart()->mapToValue(event->pos()).y()));
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

void ChartView::keepCallout()
{
	m_callouts.append(m_tooltip);
	m_tooltip = new Callout(this->chart());
}

void ChartView::tooltip(QPointF point, bool state)
{
	if (m_tooltip == 0)
		m_tooltip = new Callout(this->chart());

	if (state) {
		m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
		m_tooltip->setAnchor(point);
		m_tooltip->setZValue(11);
		m_tooltip->updateGeometry();
		m_tooltip->show();
	} else {
		m_tooltip->hide();
	}
}


void ChartView::resizeEvent(QResizeEvent *event)
{
	if (scene()) {
		scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
		this->chart()->resize(event->size());
		m_coordX->setPos(this->chart()->size().width()/2 - 50, this->chart()->size().height() - 20);
		m_coordY->setPos(this->chart()->size().width()/2 + 50, this->chart()->size().height() - 20);
		const auto callouts = m_callouts;
		for (Callout *callout : callouts)
			callout->updateGeometry();
	}
	QGraphicsView::resizeEvent(event);
}


void ChartView::wheelEvent(QWheelEvent *event)
{
	// QPoint numPixels = event->pixelDelta();
	QPoint numDegrees = event->angleDelta() / 8;

	// if (!numPixels.isNull()) {
		// qDebug() << "numPix:" << numPixels;
	// }
	if (!numDegrees.isNull()) {
		QPoint numSteps = numDegrees / 15;
		// qDebug() << "numDegr:" << numDegrees;
		if (numSteps.y() > 0) {
			// this->chart()->zoomIn();
			this->chart()->zoom(1.1);
			// this->chart()->zoomIn({event->position().x(), event->position().y(),});
		}
		else {
			this->chart()->zoom(0.9);
				// this->chart()->zoomOut();
		}
	}

	event->accept();
}

#include "chartview.h"
#include <QDebug>
#include <QMenu>
#include <QFileDialog>
#include <QSplineSeries>

ChartView::ChartView() {
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
    connect(this->settingsDialogWindow, &SettingsChartView::settingsUpdated, this, &ChartView::onSettingsUpdated);
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
        QAction* save = menu.addAction("Сохранить изображение");
        QAction* save_data = menu.addAction("Экспортировать данные");
        QAction* preferences = menu.addAction("Настройки");

        QAction* selectedAction = menu.exec(event->globalPos());

		if (selectedAction == save) {
            QString fullPath = QFileDialog::getSaveFileName(this, tr("Сохранить как..."), QString(),"*.png");
			saveContent(fullPath);
		} else if (selectedAction == save_data) {
            QString fullPath = QFileDialog::getSaveFileName(this, tr("Экспортировать как..."), QString(),"*.txt");
			exportContent(fullPath);
        } else if (selectedAction == preferences) {

            settingsDialogWindow->updateSettings(this->m_settings.getPlotSettings());
            settingsDialogWindow->open();
		}
	}
	QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
	m_coordX->setText(QString("X: %1").arg(chart()->mapToValue(event->pos()).x()));
	m_coordY->setText(QString("Y: %1").arg(chart()->mapToValue(event->pos()).y()));
	QChartView::mouseMoveEvent(event);
}

bool ChartView::event(QEvent *event)
{
	if (event->type() == QEvent::Leave) {
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

void ChartView::onSettingsUpdated(PlotSettings &p)
{
    QValueAxis *axisX;
    QValueAxis *axisY;

//    this->chart()->createDefaultAxes();
    if (this->chart()->axes().isEmpty())
        return;
    axisX = static_cast<QValueAxis *>(this->chart()->axes().at(0));
    axisY = static_cast<QValueAxis *>(this->chart()->axes(Qt::Vertical).at(0));
    axisX->setRange(p.grid.minX, p.grid.maxX);
    axisY->setRange(p.grid.minY, p.grid.maxY);
    axisX->setTickCount(p.grid.ticksX);
    axisY->setTickCount(p.grid.ticksY);

    axisX->setLabelsEditable(1);
    axisX->setLabelsVisible(1);

    axisY->setLabelsEditable(1);
    axisY->setLabelsVisible(1);

    this->settings().setPlotSettings(p);
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

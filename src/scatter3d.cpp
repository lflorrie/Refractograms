#include "scatter3d.h"
#include <QMenu>
#include <QFileDialog>


Scatter3D::Scatter3D()
{
    settingsDialogWindow = new SettingsChartView(nullptr, true);

    connect(this->settingsDialogWindow, &SettingsChartView::settingsUpdated, this, &Scatter3D::onSettingsUpdated);

}

void Scatter3D::saveContent(const QString &path)
{
	if (path.isEmpty())
		return;
	this->renderToImage(0,{1920, 1080}).save(path);
}

void Scatter3D::exportContent(const QString &path)
{
	if (path.isEmpty() || this->seriesList().isEmpty())
		return;
	QFile file(path);
	file.open(QFile::ReadWrite);
	auto dataToSave = this->seriesList().at(0)->dataProxy()->array();
	int pointCount = dataToSave->size();
	for (int i = 0; i < pointCount; i++) {
		QScatterDataItem dataItem = dataToSave->at(i);

		std::string line = QString::number(dataItem.position().x()).toStdString() + " "
						   + QString::number(dataItem.position().y()).toStdString() + " "
						   + QString::number(dataItem.position().z()).toStdString() + "\n";
		file.write(line.c_str());
	}
	file.close();
}

void Scatter3D::mousePressEvent(QMouseEvent *event)
{
	qDebug() << "Scatter3D: MousePressEvent";
	if (event->button() == Qt::MiddleButton) {
        QMenu menu;
        QAction* save = menu.addAction(tr("Сохранить изображение"));
        QAction* save_data = menu.addAction(tr("Экспортировать данные"));
        QAction* preferences = menu.addAction(tr("Настройки"));

		QAction* selectedAction = menu.exec(event->globalPos());
		// TODO: QScatter3D is not a widget, but getSaveFileName need widget parent
		if (selectedAction == save) {
            QString fullPath = QFileDialog::getSaveFileName(nullptr, tr("Сохранить как..."), QString(),"*.png");
			saveContent(fullPath);
		} else if (selectedAction == save_data) {
            QString fullPath = QFileDialog::getSaveFileName(nullptr, tr("Экспортировать как..."), QString(),"*.txt");
			exportContent(fullPath);
        } else if (selectedAction == preferences) {
            settingsDialogWindow->open();
		}
    }
    Q3DScatter::mousePressEvent(event);
}

void Scatter3D::onSettingsUpdated(PlotSettings &s)
{
    auto ax = this->axisX();
    auto ay = this->axisY();
    auto az = this->axisZ();

    ax->setRange(s.grid.minX, s.grid.maxX);
    ay->setRange(s.grid.minY, s.grid.maxY);
    az->setRange(s.grid.minZ, s.grid.maxZ);
    ax->setSegmentCount(s.grid.ticksX);
    ay->setSegmentCount(s.grid.ticksY);
    az->setSegmentCount(s.grid.ticksZ);
    m_settings.setPlotSettings(s);
}

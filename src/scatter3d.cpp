#include "scatter3d.h"
#include <QMenu>
#include <QFileDialog>


Scatter3D::Scatter3D()
{

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
		QAction* save = menu.addAction("Save");
		QAction* save_data = menu.addAction("Save data");
		QAction* preferences = menu.addAction("Preferences");

		QAction* selectedAction = menu.exec(event->globalPos());
		// TODO: QScatter3D is not a widget, but getSaveFileName need widget parent
		if (selectedAction == save) {
			QString fullPath = QFileDialog::getSaveFileName(nullptr, tr("Save as..."), QString(),"*.png");
			saveContent(fullPath);
		} else if (selectedAction == save_data) {
			QString fullPath = QFileDialog::getSaveFileName(nullptr, tr("Export as..."), QString(),"*.txt");
			exportContent(fullPath);
		} else if (selectedAction == preferences) {
			// settingsDialogWindow.open();
		}
	}
	Q3DScatter::mousePressEvent(event);
}

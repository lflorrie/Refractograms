#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "refrlogic1.h"
#include <QtDebug>
#include <QMessageBox>
#include <QValueAxis>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	m_font.setBold(1);
	m_font.setPixelSize(20);
    setWindowTitle("Refracrograms");
	progressBar = new QProgressBar();
	progressBar->setRange(0, 100);
	progressBar->setValue(0);
	statusBar()->addPermanentWidget(progressBar);

	// Эффект прозрачности
	opacityEffect = new QGraphicsOpacityEffect(statusBar());
	statusBar()->setGraphicsEffect(opacityEffect);

	// Анимация для эффекта прозрачности
	animationProgressBar = new QPropertyAnimation(opacityEffect, "opacity");
	animationProgressBar->setDuration(1000);  // Продолжительность анимации 1 секунда
	animationProgressBar->setStartValue(1.0);  // Начальная непрозрачность
	animationProgressBar->setEndValue(0.0);    // Конечная непрозрачность

	std::vector<QLayout *> layouts;
	layouts.push_back(ui->tab->layout());
	layouts.push_back(ui->tab_2->layout());
	layouts.push_back(ui->tab_3->layout());
	layouts.push_back(ui->tab_4->layout());

	m_charts = new RefrCharts(layouts, &m_font);

	qInfo() << "Main window created";
	connect(ui->actionSave_all, &QAction::triggered, this, &MainWindow::on_actionSaveAll);
	connect(ui->actionExport_data, &QAction::triggered, this, &MainWindow::on_actionExportData);

	connect(m_charts, &RefrCharts::progressChanged, this, &MainWindow::updateProgress);
	connect(m_charts, &RefrCharts::finished, this, &MainWindow::taskFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
	qInfo() << "Main window deleted";
}

void MainWindow::on_pushButton_clicked()
{
	qInfo() << "Button clicked. Updating fields for func_t and func_n.";
	QString tab = ui->comboBox->currentText();
	Plots p;
	if (tab == "All")
		p = ALL_PLOTS;
	if (tab == "3.1")
		p = PLOT_3_1;

	m_charts->buildPlots(getValuesFromInput(), p);
}

void MainWindow::on_actionSaveAll()
{
	qInfo() << "Action SaveAll";
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select the folder to save"));

	if (!folderPath.isEmpty()) {
		qInfo() << "Save all to:" << folderPath;
		int currentTabIndex = ui->tabWidget->currentIndex();

		for (int i = 0; i < TAB_MAX; ++i)
		{
			int		chartTabIndex = ui->tabWidget->indexOf(m_charts->chartViews[i]->parentWidget());
			QString filename = QString("%1/test%2.png").arg(folderPath).arg(i);

			ui->tabWidget->setCurrentIndex(chartTabIndex);
			m_charts->chartViews[i]->saveContent(filename);
		}
		m_charts->scatter3d->saveContent(QString("%1/test3d.png").arg(folderPath));
		ui->tabWidget->setCurrentIndex(currentTabIndex);
	}
}

void MainWindow::on_actionExportData()
{
	qInfo() << "Action ExportData";
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select the folder to save"));

	if(!folderPath.isEmpty()){
		qInfo() << "Export all data to:" << folderPath;
		for (int i = 0; i < TAB_MAX; ++i)
		{
			QString filename = QString("%1/refr_data%2.txt").arg(folderPath).arg(i);
			m_charts->chartViews[i]->exportContent(filename);
		}
		QString filename = QString("%1/refr_data3d.txt").arg(folderPath);
		m_charts->scatter3d->exportContent(filename);
	}
}
void MainWindow::updateProgress(int value)
{
	qInfo() << "Update progress:" << value;
	progressBar->setValue(value);
	opacityEffect->setOpacity(1);
}

void MainWindow::taskFinished()
{
	qInfo() << "Task Finished";
	animationProgressBar->start();
	// progressBar->setVisible(false);
}

RefrLogicData MainWindow::getValuesFromInput()
{
    RefrLogicData data;
	data.R		= ui->lineEdit->text().toDouble();
	data.a		= ui->lineEdit_2->text().toDouble();
	data.z0		= ui->lineEdit_3->text().toDouble();
    data.deltaR = ui->lineEdit_4->text().toDouble();
	data.z1		= ui->lineEdit_5->text().toDouble();
	data.T0		= ui->lineEdit_6->text().toDouble();
    data.deltaT = ui->lineEdit_7->text().toDouble();
	data.x0		= ui->lineEdit_8->text().toDouble();

	data.z_settings.count = ui->li_count_z->text().toInt();
	data.z_settings.count_of_points = ui->li_count_points->text().toInt();
	data.z_settings.current = ui->li_current_z->text().toDouble();
	data.z_settings.start = ui->li_start->text().toDouble();
	data.z_settings.step = ui->li_step->text().toDouble();
    return data;
}


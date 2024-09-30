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

	std::vector<QLayout *> layouts;
	layouts.push_back(ui->tab->layout());
	layouts.push_back(ui->tab_2->layout());
	layouts.push_back(ui->tab_3->layout());
	layouts.push_back(ui->tab_4->layout());

	m_charts = new RefrCharts(layouts, &m_font);

	// plot2DPlotRefr1();
	qInfo() << "Main window created";
	connect(ui->actionSave_all, &QAction::triggered, this, &MainWindow::on_actionSaveAll);
}

MainWindow::~MainWindow()
{
    delete ui;
	qInfo() << "Main window deleted";
}

void MainWindow::on_pushButton_clicked()
{
	qInfo() << "Button clicked. Updating fields for func_t and func_n.";
	m_charts->plot2DPlotRefr1(getValuesFromInput());
}

void MainWindow::on_actionSaveAll()
{
	qInfo() << "Action SaveAll";
	QFileDialog objFlDlg(this);
	// objFlDlg.setOption(QFileDialog::ShowDirsOnly, true);
	objFlDlg.setAcceptMode(QFileDialog::AcceptSave);

	QList<QLineEdit *> lst =objFlDlg.findChildren<QLineEdit *>();
	qDebug() << lst.count();
	if(lst.count()==1){
		lst.at(0)->setReadOnly(true);
	}else{
		//Need to be handled if more than one QLineEdit found
	}
	if(objFlDlg.exec()){
		qInfo() << "Save to:" << objFlDlg.directory().absolutePath();
		int currentTabIndex = ui->tabWidget->currentIndex();
		for (int i = 0; i < m_charts->TAB_MAX; ++i)
		{
			int chartTabIndex = ui->tabWidget->indexOf(m_charts->chartViews[i]->parentWidget());
			ui->tabWidget->setCurrentIndex(chartTabIndex);

			auto size = m_charts->charts[i]->size();
			auto sizeV = m_charts->chartViews[i]->size();
			m_charts->charts[i]->resize(1920, 1080);
			m_charts->chartViews[i]->resize(1920, 1080);

			QString filename = QString("%1/test%2.png").arg(objFlDlg.directory().absolutePath()).arg(i);
			m_charts->chartViews[i]->grab({0, 0, 1920, 1080}).save(filename);

			m_charts->charts[i]->resize(size);
			m_charts->chartViews[i]->resize(sizeV);

		}
		m_charts->scatter3d->renderToImage(0,{1920, 1080}).save(QString("%1/test3d.png").arg(objFlDlg.directory().absolutePath()));
		ui->tabWidget->setCurrentIndex(currentTabIndex);
	}
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


#include "settingschartview.h"
#include "ui_settingschartview.h"

// SettingsChartView::SettingsChartView() {}

SettingsChartView::SettingsChartView(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsChartView)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Настройки параметров графика"));
}

void SettingsChartView::updateSettings(const PlotSettings &s)
{
    ui->le_minX->setText(QString::number(s.grid.minX));
    ui->le_maxX->setText(QString::number(s.grid.maxX));
    ui->le_minY->setText(QString::number(s.grid.minY));
    ui->le_maxY->setText(QString::number(s.grid.maxY));
    ui->le_ticksX->setText(QString::number(s.grid.ticksX));
    ui->le_ticksY->setText(QString::number(s.grid.ticksY));
}

void SettingsChartView::on_cancelButton_clicked()
{
    this->close();
}


void SettingsChartView::on_saveButton_clicked()
{
    on_applyButton_clicked();
    this->close();
}

void SettingsChartView::on_applyButton_clicked()
{
    PlotSettings s;
    s.grid.minX = ui->le_minX->text().toDouble();
    s.grid.maxX = ui->le_maxX->text().toDouble();
    s.grid.minY = ui->le_minY->text().toDouble();
    s.grid.maxY = ui->le_maxY->text().toDouble();
    s.grid.ticksX = ui->le_ticksX->text().toDouble();
    s.grid.ticksY = ui->le_ticksY->text().toDouble();
    emit settingsUpdated(s);
}


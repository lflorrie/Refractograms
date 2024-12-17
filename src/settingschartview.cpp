#include "settingschartview.h"
#include "ui_settingschartview.h"

// SettingsChartView::SettingsChartView() {}

SettingsChartView::SettingsChartView(QWidget *parent, bool enable3d) : QDialog(parent), ui(new Ui::SettingsChartView), m_enable3d(enable3d)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Настройки параметров графика"));
    if (!m_enable3d)
    {
        ui->le_minZ->setVisible(false);
        ui->le_maxZ->setVisible(false);
        ui->le_ticksZ->setVisible(false);
        ui->label_10_z->setVisible(false);
        ui->label_11_z->setVisible(false);
        ui->label_12_z->setVisible(false);
        ui->label_13_z->setVisible(false);
    }
}

void SettingsChartView::updateSettings(const PlotSettings &s)
{
    ui->le_minX->setText(QString::number(s.grid.minX));
    ui->le_maxX->setText(QString::number(s.grid.maxX));
    ui->le_minY->setText(QString::number(s.grid.minY));
    ui->le_maxY->setText(QString::number(s.grid.maxY));
    ui->le_ticksX->setText(QString::number(s.grid.ticksX));
    ui->le_ticksY->setText(QString::number(s.grid.ticksY));
    if (m_enable3d)
    {
        ui->le_minZ->setText(QString::number(s.grid.minZ));
        ui->le_maxZ->setText(QString::number(s.grid.maxZ));
        ui->le_ticksZ->setText(QString::number(s.grid.ticksZ));
    }
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
    if (m_enable3d)
    {
        s.grid.minZ = ui->le_minZ->text().toDouble();
        s.grid.maxZ = ui->le_maxZ->text().toDouble();
        s.grid.ticksZ = ui->le_ticksZ->text().toDouble();
    }
    emit settingsUpdated(s);
}


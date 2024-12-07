#include "settingschartview.h"
#include "ui_settingschartview.h"

// SettingsChartView::SettingsChartView() {}

SettingsChartView::SettingsChartView(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsChartView)
{
	ui->setupUi(this);
}

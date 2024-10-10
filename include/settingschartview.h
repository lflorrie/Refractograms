#ifndef SETTINGSCHARTVIEW_H
#define SETTINGSCHARTVIEW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsChartView; }
QT_END_NAMESPACE

class SettingsChartView : public QDialog
{
	Q_OBJECT
public:
	// SettingsChartView();
	SettingsChartView(QWidget *parent = nullptr);
private:
	Ui::SettingsChartView* ui;
};

#endif // SETTINGSCHARTVIEW_H

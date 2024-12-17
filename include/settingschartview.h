#ifndef SETTINGSCHARTVIEW_H
#define SETTINGSCHARTVIEW_H

#include <QDialog>
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsChartView; }
QT_END_NAMESPACE

class SettingsChartView : public QDialog
{
	Q_OBJECT
public:
	// SettingsChartView();
    SettingsChartView(QWidget *parent = nullptr, bool enable3d = false);
    void updateSettings(const PlotSettings &s);
signals:
    void settingsUpdated(PlotSettings &s);
private slots:
    void on_cancelButton_clicked();

    void on_saveButton_clicked();
    void on_applyButton_clicked();

private:
	Ui::SettingsChartView* ui;
    bool m_enable3d;
};

#endif // SETTINGSCHARTVIEW_H

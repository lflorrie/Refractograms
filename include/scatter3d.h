#ifndef SCATTER3D_H
#define SCATTER3D_H
#include <Q3DScatter>
#include <settingschartview.h>
class Scatter3D : public Q3DScatter
{
	Q_OBJECT
public:
	Scatter3D();
	void saveContent(const QString &full_path);
	void exportContent(const QString &full_path);
    Settings &settings()
    {
        return m_settings;
    }
    void updateSettingsUi(PlotSettings &s)
    {
        settingsDialogWindow->updateSettings(s);
    }
protected:
	virtual void mousePressEvent(QMouseEvent *) override;
public slots:
    void onSettingsUpdated(PlotSettings &s);
private:
    SettingsChartView *settingsDialogWindow;
    Settings m_settings;
};

#endif // SCATTER3D_H

#ifndef SETTINGS_H
#define SETTINGS_H
#include <vector>
#include <string>

struct PlotSettings
{
    /* Настройка сетки */
    struct GridSettings
    {
        GridSettings() : minX(0), maxX(0), minY(0), maxY(0), ticksX(0), ticksY(0), minZ(0), maxZ(0), ticksZ(0) {}

        double minX;
        double maxX;
        double minY;
        double maxY;

        double ticksX;
        double ticksY;

        double minZ;
        double maxZ;
        double ticksZ;
    } grid;
};

class Settings
{
public:
	Settings();

    void setPlotSettings(const PlotSettings &s);

    PlotSettings getPlotSettings() const;
private:
    PlotSettings plotSettings;
};

#endif // SETTINGS_H

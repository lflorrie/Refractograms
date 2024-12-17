#include "settings.h"

Settings::Settings()
{
}

void Settings::setPlotSettings(const PlotSettings &s)
{
    plotSettings.grid.minX = s.grid.minX;
    plotSettings.grid.maxX = s.grid.maxX;
    plotSettings.grid.minY = s.grid.minY;
    plotSettings.grid.maxY = s.grid.maxY;
    plotSettings.grid.ticksX = s.grid.ticksX;
    plotSettings.grid.ticksY = s.grid.ticksY;
    plotSettings.grid.minZ = s.grid.minZ;
    plotSettings.grid.maxZ = s.grid.maxZ;
    plotSettings.grid.ticksZ = s.grid.ticksZ;
}

PlotSettings Settings::getPlotSettings() const
{
    return plotSettings;
}

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
}

PlotSettings Settings::getPlotSettings() const
{
    return plotSettings;
}

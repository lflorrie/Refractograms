#ifndef SETTINGS_H
#define SETTINGS_H
#include <vector>
#include <string>

class PlotSettings
{
/*

	Настройка сетки
*/
};

class Settings
{
public:
	Settings();

	struct Tab {
		std::vector<double> fields;
		std::vector<PlotSettings> plotSettings;
	};

	void Save(std::string &path);
	void Load(std::string &path);
	void f() {
		settingsTabs[0].fields;
	}
private:
	std::vector<Tab> settingsTabs;
};

#endif // SETTINGS_H

#ifndef REFRLOGIC1_H
#define REFRLOGIC1_H
#include <QtMath>
#include <QtDebug>
#include "math_utils.h"
#include <chrono>
#include <QPointF>

struct RefrLogicData
{
	/* радиус шара */
	double R;
	/* характерная толщина слоя */
	double a;
	/* координата входа лазерного пучка в неоднородность */
	double z0;
    double deltaR;
    double z1;
    double T0;
    double deltaT;
    double x0;

	// Additional params
	struct SettingsForZ{
		int    count;
		int    count_of_points;
		double start;
		double step;
		double current;
	} z_settings;
};

class RefrLogic1
{
public:
    RefrLogic1();
	void initTestData();
	void testFunc();

    // в следующей формуле численно задаются параметры зависимости температуры в слое от радиуса.
	double func_t(double r) const;
    // при заданной температурной зависимости находим зависимость показателя преломления
	double func_n(double r) const;

	double func_x0(double fi, double d);
	double func_tmp(double rn, double fi, double d);

	double func_rn(double fi, double d);

    //

	double func_r0(double fi, double d);
	double func_alpha0(double fi, double d);

	double func_integr(double r, double fi, double d);

	double func_integr_new(double r);

	double func_alphan(double fi, double d);

	double func_alpha1(double r1, double fi, double d);

	double func_x1(double r1, double fi, double d);
	double func_z1(double r1, double fi, double d);

	double func_alpha2(double r2, double fi, double d);

	double func_x2(double r2, double fi, double d);
	double func_z2(double r2, double fi, double d);

	double func_r(double fi, double d, double z);

	double func_x(double fi, double d, double z);
	double func_y(double fi, double d, double z);

	std::vector<QPointF> make2DPlot(std::function<double(double)> func, double from, double to, int steps);

	void calculate();
	double getR() const;
	RefrLogicData getData() const;
	void setData(const RefrLogicData &new_data);
private:
    RefrLogicData data;
	int N_INTEGRAL = 10000;
    double fi;
    double d;
};

#endif // REFRLOGIC1_H

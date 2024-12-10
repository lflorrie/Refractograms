#include "refrlogic1.h"

RefrLogic1::RefrLogic1()
{
    qDebug() << "RefrLogic1 created";
    initTestData();
}

void RefrLogic1::initTestData() {
	data.R = 18;
	data.a = 3.680;
	data.z0 = 300;
	data.deltaR = -1.669;
	data.T0 = 180;
	data.deltaT = -150;
	data.x0 = 20;

	data.z_settings.start = 25;
	data.z_settings.step = 25;
	data.z_settings.count = 3;
	data.z_settings.count_of_points = 100;
	data.z_settings.current = 25;
}

void RefrLogic1::testFunc() {
	qDebug() << "TEST!";
	auto startTime = std::chrono::steady_clock::now();
	double n0 = func_n(0);
	qDebug() << "1.33134 -" << n0;
	qDebug() << "22.00016106 -" << func_rn(0, 22);
	qDebug() << "300.0 -" << func_r0(0, 0);

	qDebug() << "3.141559320256472 -" << func_alpha0(0, 0.01);
	qDebug() << "1.570795939558606 -" << func_alphan(0, 0.01);

	qDebug() << "300.16996851783824 -" << func_r0(M_PI / 3, 5 * data.a);
	qDebug() << "3.0915717967840233 -" << func_alpha1(40, 0, 2);
	qDebug() << "0.049864396535265865 -" << func_alpha2(40, 0, 2);
	qDebug() << "2.10362812 -" << func_r(1, 1, 1);

	auto endTime = std::chrono::steady_clock::now();
	qDebug() << "\n----------------------------------------";
	qDebug() << "Calculation TIME:" << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << "mcs";
}

double RefrLogic1::func_t(double r) const{
	return data.T0 + data.deltaT * qExp((-qPow(r - data.R - data.deltaR, 2)) / (qPow(data.a, 2)));
}

double RefrLogic1::func_n(double r) const {
	return 1.3328 - 0.000051 * func_t(r) - 0.0000011 * qPow(func_t(r), 2);
}

double RefrLogic1::func_x0(double fi, double d) {
	return d / qCos(fi);
}

double RefrLogic1::func_tmp(double rn, double fi, double d) {
	double n0 = func_n(0);
	return func_n(rn) * rn - n0 * func_x0(fi, d);
}

double RefrLogic1::func_rn(double fi, double d) {
	auto tempFunc = [&](double r){ return func_tmp(r, fi, d); };
	return FindRoot(tempFunc, 0, 6 * data.R); // TODO: replace to boost func
}

double RefrLogic1::func_r0(double fi, double d) {
	return qSqrt(qPow(func_x0(fi, d), 2) + qPow(data.z0, 2));
}

double RefrLogic1::func_alpha0(double fi, double d) {
	return M_PI_2 + qAtan(data.z0 / func_x0(fi, d));
}

double RefrLogic1::func_integr(double r, double fi, double d) {
	double n0 = func_n(0);
	return (n0 * func_x0(fi, d)) /
		   (r * qSqrt(qPow(func_n(r), 2) * qPow(r, 2) - qPow(n0, 2) * qPow(func_x0(fi, d), 2)));
}

double RefrLogic1::func_integr_new(double r) {
	double n0 = func_n(0);
	return (n0 * func_x0(fi, d)) /
		   (r * qSqrt(qPow(func_n(r), 2) * qPow(r, 2) - qPow(n0, 2) * qPow(func_x0(fi, d), 2)));
}

double RefrLogic1::func_alphan(double fi, double d) {
	this->fi = fi;
	this->d = d;
	auto integr = [&](double r){ return func_integr(r, fi, d); };
	try  {
		// return func_alpha0(fi, d) + simpsonIntegral<decltype(integr)>(integr, func_r0(fi, d), func_rn(fi, d), N_INTEGRAL);
		return func_alpha0(fi, d) + boost::math::quadrature::gauss_kronrod<double, 15>::integrate(integr, func_r0(fi, d), func_rn(fi, d), 15, 1e-9);
	} catch (std::exception &ex){
		qDebug() << "ALPHA N";
		exit(1);
	}
}

double RefrLogic1::func_alpha1(double r1, double fi, double d) {
	this->fi = fi;
	this->d = d;
	auto integr = [&](double r){ return func_integr(r, fi, d); };
	try {
		// return func_alpha0(fi, d) + simpsonIntegral<decltype(integr)>(integr, func_r0(fi, d), r1, N_INTEGRAL);
		return func_alpha0(fi, d) + boost::math::quadrature::gauss_kronrod<double, 15>::integrate(integr, func_r0(fi, d), r1, 15, 1e-9);
	} catch (std::exception &ex){
		qDebug() << "ALPHA 1";
		exit(1);
	}
}

double RefrLogic1::func_x1(double r1, double fi, double d) {
	return r1 * qSin(func_alpha1(r1, fi, d));
}

double RefrLogic1::func_z1(double r1, double fi, double d) {
	return r1 * qCos(func_alpha1(r1, fi, d));
}

double RefrLogic1::func_alpha2(double r2, double fi, double d) {
	auto integr = [&](double r){ return func_integr(r, fi, d); };
	try {
		// return func_alphan(fi, d) + simpsonIntegral<decltype(integr)>(integr, r2, func_rn(fi, d), N_INTEGRAL);
		return func_alphan(fi, d) + boost::math::quadrature::gauss_kronrod<double, 15>::integrate(integr,  r2, func_rn(fi, d), 15, 1e-9);
	} catch (std::exception &ex){
		qDebug() << "ALPHA 2" << r2 << func_rn(fi, d);
		exit(2);
	}

}

double RefrLogic1::func_x2(double r2, double fi, double d) {
	return r2 * qSin(func_alpha2(r2, fi, d));
}

double RefrLogic1::func_z2(double r2, double fi, double d) {
	return r2 * qCos(func_alpha2(r2, fi, d));
}

double RefrLogic1::func_r(double fi, double d, double z) {
	auto func_tmp = [&](double r) { return func_z2(r, fi, d) - z; };
	// boost::math::tools::eps_tolerance<double> tol(std::numeric_limits<double>::digits - 3);
	// boost::uintmax_t it = 20;
	// auto res = boost::math::tools::toms748_solve(func_tmp, 0.0, data.R * 8, [](double a, double b) { return (b - a) < 0.001; }, it);
	// return res.first;
	// try {
	//  auto res = boost::math::tools::bracket_and_solve_root(func_tmp, z * 3, 2.0, true, [](double a, double b) { return std::abs(b - a) < 0.01; }, it);
	//  qDebug () << "RES.FIRST" << res.first;
	//  return res.first;
	// } catch (std::exception &ex){
	//  qDebug() << "SOLVEEEE!" << ex.what();
	//  qDebug() << fi << d << z;
	//  exit (1);
	// }
	return FindRoot(func_tmp, z, z * 3);
}

double RefrLogic1::func_x(double fi, double d, double z) {
	return func_r(fi, d, z) * qSin(func_alpha2(func_r(fi, d, z), fi, d)) * qCos(fi);
}

double RefrLogic1::func_y(double fi, double d, double z) {
	return func_r(fi, d, z) * qSin(func_alpha2(func_r(fi, d, z), fi, d)) * qSin(fi);
}

std::vector<QPointF> RefrLogic1::make2DPlot(std::function<double (double)> func, double from, double to, int steps) {
	std::vector<QPointF> res;
	double curVal = from;
	double step = (to - from) / steps;

	for (int i = 0; i <= steps; ++i) {
		res.push_back(QPointF(curVal, func(curVal)));
		curVal += step;
	}
	return res;
}

double RefrLogic1::getR() const{
	return data.R;
}

RefrLogicData RefrLogic1::getData() const {
	return data;
}

void RefrLogic1::setData(const RefrLogicData &new_data)
{
	data.R = new_data.R;
	data.T0 = new_data.T0;
	data.a = new_data.a;
	data.deltaR = new_data.deltaR;
	data.deltaT = new_data.deltaT;
	data.x0 = new_data.x0;
	data.z0 = new_data.z0;

	data.z_settings = new_data.z_settings;
}

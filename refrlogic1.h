#ifndef REFRLOGIC1_H
#define REFRLOGIC1_H
#include <QtMath>
#include <QtDebug>
#include "math_utils.h"
#include <chrono>
#include <thread>
#include <QPointF>
class RefrLogic1
{
public:
    RefrLogic1();
    void initTestData() {
        R = 50.5 / 2;
        a = 1.01;
        z0 = 300;
    }
    void testFunc() {
        qDebug() << "TEST!";
        auto startTime = std::chrono::steady_clock::now();
        double n0 = func_n(0);
        qDebug() << "1.33134 -" << n0;
        qDebug() << "22.00016106 -" << func_rn(0, 22);
        qDebug() << "300.0 -" << func_r0(0, 0);

        qDebug() << "3.141559320256472 -" << func_alpha0(0, 0.01);
        qDebug() << "1.570795939558606 -" << func_alphan(0, 0.01);

        qDebug() << "300.16996851783824 -" << func_r0(M_PI / 3, 5 * a);
        qDebug() << "3.0915717967840233 -" << func_alpha1(40, 0, 2);
        qDebug() << "0.049864396535265865 -" << func_alpha2(40, 0, 2);
        qDebug() << "2.10362812 -" << func_r(1, 1, 1);

        auto endTime = std::chrono::steady_clock::now();
        qDebug() << "\n----------------------------------------";
        qDebug() << "Calculation TIME:" << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << "mcs";

    }

    // в следующей формуле численно задаются параметры зависимости температуры в слое от радиуса.
    inline double func_t(double r) const{
        return 20.0 + 100.0 * qExp((-qPow(r - R + 0.6, 2)) / (qPow(a, 2)));
    }
    // при заданной температурной зависимости находим зависимость показателя преломления
    inline double func_n(double r) const {
        return 1.3328 - 0.000051 * func_t(r) - 0.0000011 * qPow(func_t(r), 2);
    }

    inline double func_x0(double fi, double d) {
        return d / qCos(fi);
    }
    inline double func_tmp(double rn, double fi, double d) {
        double n0 = func_n(0);
        return func_n(rn) * rn - n0 * func_x0(fi, d);
    }

    inline double func_rn(double fi, double d) {
        auto tempFunc = [&](double r){ return func_tmp(r, fi, d); };
        return FindRoot(tempFunc, 0, 6 * R); // TODO: replace to boost func
    }

    //

    inline double func_r0(double fi, double d) {
        return qSqrt(qPow(func_x0(fi, d), 2) + qPow(z0, 2));
    }
    inline double func_alpha0(double fi, double d) {
        return M_PI_2 + qAtan(z0 / func_x0(fi, d));
    }

    inline double func_integr(double r, double fi, double d) {
        double n0 = func_n(0);
        return (n0 * func_x0(fi, d)) /
               (r * qSqrt(qPow(func_n(r), 2) * qPow(r, 2) - qPow(n0, 2) * qPow(func_x0(fi, d), 2)));
    }

    inline double func_integr_new(double r) {
        double n0 = func_n(0);
        return (n0 * func_x0(fi, d)) /
               (r * qSqrt(qPow(func_n(r), 2) * qPow(r, 2) - qPow(n0, 2) * qPow(func_x0(fi, d), 2)));
    }

    inline double func_alphan(double fi, double d) {
        this->fi = fi;
        this->d = d;
        auto integr = [&](double r){ return func_integr(r, fi, d); };
        try  {
        return func_alpha0(fi, d) + boost::math::quadrature::gauss_kronrod<double, 61>::integrate(integr, func_r0(fi, d), func_rn(fi, d));
        } catch (std::exception &ex){
            qDebug() << "ALPHA N";
            exit(1);
        }
    }

    inline double func_alpha1(double r1, double fi, double d) {
        this->fi = fi;
        this->d = d;
        auto integr = [&](double r){ return func_integr(r, fi, d); };
        try {
        return func_alpha0(fi, d) + boost::math::quadrature::gauss_kronrod<double, 61>::integrate(integr, func_r0(fi, d), r1);
        } catch (std::exception &ex){
            qDebug() << "ALPHA 1";
            exit(1);
        }
    }

    inline double func_x1(double r1, double fi, double d) {
        return r1 * qSin(func_alpha1(r1, fi, d));
    }
    inline double func_z1(double r1, double fi, double d) {
        return r1 * qCos(func_alpha1(r1, fi, d));
    }

    inline double func_alpha2(double r2, double fi, double d) {
        auto integr = [&](double r){ return func_integr(r, fi, d); };
        try {
            return func_alphan(fi, d) + boost::math::quadrature::gauss_kronrod<double, 61>::integrate(integr,  r2, func_rn(fi, d));
        } catch (std::exception &ex){
            qDebug() << "ALPHA 2" << r2 << func_rn(fi, d);
            exit(2);
        }
    }

    inline double func_x2(double r2, double fi, double d) {
        return r2 * qSin(func_alpha2(r2, fi, d));
    }
    inline double func_z2(double r2, double fi, double d) {
        return r2 * qCos(func_alpha2(r2, fi, d));
    }

    inline double func_r(double fi, double d, double z) {
        auto func_tmp = [&](double r) { return func_z2(r, fi, d) - z; };
//        boost::math::tools::eps_tolerance<double> tol(std::numeric_limits<double>::digits - 3);
//        boost::uintmax_t it = 20;
//        auto res = boost::math::tools::toms748_solve(func_tmp, 0.0, R * 8, [](double a, double b) { return (b - a) < 0.001; }, it);
//        try {
//            auto res = boost::math::tools::bracket_and_solve_root(func_tmp, z * 3, 2.0, true, [](double a, double b) { return std::abs(b - a) < 0.01; }, it);
//            qDebug () << "RES.FIRST" << res.first;
//            return res.first;
//        } catch (std::exception &ex){
//            qDebug() << "SOLVEEEE!" << ex.what();
//            qDebug() << fi << d << z;
//            exit (1);
//        }
        return FindRoot(func_tmp, z * 1.5, z * 3);
    }

    inline double func_x(double fi, double d, double z) {
        return func_r(fi, d, z) * qSin(func_alpha2(func_r(fi, d, z), fi, d)) * qCos(fi);
    }
    inline double func_y(double fi, double d, double z) {
        return func_r(fi, d, z) * qSin(func_alpha2(func_r(fi, d, z), fi, d)) * qSin(fi);
    }

    std::vector<QPointF> makePlotFuncT(double from, double to, int steps) {
        std::vector<QPointF> res;
        double curVal = from;
        double step = (to - from) / steps;

        for (int i = 0; i <= steps; ++i) {
            res.push_back(QPointF(curVal, func_t(curVal)));
            curVal += step;
        }
        return res;
    }

    std::vector<QPointF> makePlotFuncN(double from, double to, int steps) {
        std::vector<QPointF> res;
        double curVal = from;
        double step = (to - from) / steps;

        for (int i = 0; i <= steps; ++i) {
            res.push_back(QPointF(curVal, func_n(curVal)));
            curVal += step;
        }
        return res;
    }
//    template<typename T>
    std::vector<QPointF> make2DPlot(std::function<double(double)> func, double from, double to, int steps) {
        std::vector<QPointF> res;
        double curVal = from;
        double step = (to - from) / steps;

        for (int i = 0; i <= steps; ++i) {
            res.push_back(QPointF(curVal, func(curVal)));
            curVal += step;
        }
        return res;
    }


    void calculate() {
//        testFunc();
        auto startTime = std::chrono::steady_clock::now();


//        fi = np.linspace(-math.pi/4, math.pi/4, 100)
//        x = [func_x(i, 25.5, 40) for i in fi]
//        y = [func_y(i, 25.5, 40) for i in fi]
        std::vector<QPointF> points;
        double fi_min = -M_PI_4;
        double fi_max = M_PI_4;
        double fi_step = (fi_max - fi_min) / 100;
        double d = 25.5;
        double z = 40;


        double fi = fi_min;
//        for (int i = 0; i < 100; ++i) {
            QPointF point;
            point.setX(func_x(fi, d, z));
            point.setY(func_y(fi, d, z));
            points.push_back(point);
            fi += fi_step;
//        }

        auto endTime = std::chrono::steady_clock::now();
        qDebug() << "\n----------------------------------------";
        qDebug() << "Calculation TIME:" << std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count() << "s"
                 << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << "mcs";

    }
    double getR() const{
        return R;
    }
private:
    double R; // радиус шара
    double a; // характерная толщина слоя
    int z0; // координата входа лазерного пучка в неоднородность

    double fi;
    double d;

    int N_INTEGRAL = 1000;
    /* 1 000 000
        1.33134 - 1.33134
        22.00016106 - 22.0002
        300.0 - 300
        3.141559320256472 - 3.14156
        1.570795939558606 - 1.64529
        300.16996851783824 - 300.17
        3.0915717967840233 - 3.09157
        0.049864396535265865 - 0.0569293
        2.10362812 - 1.88127e-07
        Calculation TIME: 64065191 mcs
     */
    /* 1 000
        1.33134 - 1.33134
        22.00016106 - 22.0002
        300.0 - 300
        3.141559320256472 - 3.14156
        1.570795939558606 - 2.9939
        300.16996851783824 - 300.17
        3.0915717967840233 - 3.09157
        0.049864396535265865 - 0.277023
        2.10362812 - 1.88127e-07
        Calculation TIME: 69173 mcs
     */
};

#endif // REFRLOGIC1_H

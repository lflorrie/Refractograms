#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <vector>

template <typename T>
double FindRoot(T f, double a, double b, double eps = 0.0000001){
    double c = 0;
    while((b - a) / 2 >= eps){
        c = (a + b) / 2;
        if((f(a) * f(c)) > 0)
            a = c;
        else
            b = c;
    }
    return c;
}

template <typename T>
double simpsonIntegral(double a, double b, int n, T *obj, double (T::*f)(double, double ), double phi) // Расчет интегралла
{
    //fake Simpson
    double step = (b - a) / n;  // width of each small rectangle
    double area = 0.0;  // signed area
    for (int i = 0; i < n; i ++) {
        area += (obj->*f)(a + (i + 0.5) * step, phi) * step; // sum up each small rectangle
    }
    return area;
}

template <typename T>
double simpsonIntegral(T f, double a, double b, int n) // Расчет интегралла
{
    //fake Simpson
    double step = (b - a) / n;  // width of each small rectangle
    double area = 0.0;  // signed area
    for (int i = 0; i < n; i ++) {
        area += f(a + (i + 0.5) * step) * step; // sum up each small rectangle
    }
    return area;
}

#endif // MATH_UTILS_H

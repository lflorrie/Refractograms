#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QChart>
#include <QtDataVisualization>
#include "refrlogic1.h"
#include <QXYSeries>
#include <QSplineSeries>
#include <QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void plot2DPlotRefr1();

    int a;
private:
    RefrLogic1 refr1;
    Ui::MainWindow *ui;
};

//class massive {
//public:

//    massive() {
//        n = 0;
//    }
//    massive(std::vector<int> v, int size) {
//        data = v;
//        n = size;
//    }

//    int size() {
//        return n;
//    }
//    void addElement(int a) {
//        data.push_back(a);
//        n++;
//    }
//    void print() {
//        // print
//    }
//private:
//    int n; // размер массива
//    std::vector<int> data;
//};



//void print(std::vector<int> v, int minVal) {
// // print
//}

//void push(std::vector<int> &v, int val) {
//    v.push_back(val);
//    n++;
//}

//void func() {
//    massive m;
////    massive m({1, 2 , 3}, 3);
////    m.data = std::vector<int> {1,2, 3};
//    m.print();
////    std::vector<int> data;

////    push(data, 10);

////    print(data, minVal);

////    calculate(data, minVal);

////    push(data, 10);

////    std::vector<int> data;






//    massive m;
//    m.n = 10;
//    m.size(); // 10
////    m.addElement(10);


//}



#endif // MAINWINDOW_H

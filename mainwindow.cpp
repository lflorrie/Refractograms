#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "refrlogic1.h"
#include <QtDebug>

#include "scattergraph.h"
#include "surfacegraph.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    RefrLogic1 refr1;
    qDebug() << "OK";


    const QSize screenSize = ui->tabWidget->screen()->size();
    const QSize minimumGraphSize{screenSize.width() / 2, qRound(screenSize.height() / 1.75)};

    // Create scatter graph
    ScatterGraph scatter;
    // Create surface graph
    SurfaceGraph surface;

    // Add scatter widget
    if (!scatter.initialize(minimumGraphSize, screenSize)
        || !surface.initialize(minimumGraphSize, screenSize)) {
        QMessageBox::warning(nullptr, "Graph Gallery", "Couldn't initialize the OpenGL context.");
//        return -1;
    }
    ui->tabWidget->addTab(scatter.scatterWidget(), "Scatter Graph");
    // Add surface widget
//     ui->tabWidget->addTab(surface.surfaceWidget(), "Surface Graph");
}

MainWindow::~MainWindow()
{
    delete ui;
}


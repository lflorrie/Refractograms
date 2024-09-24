QT += core gui charts
QT += datavisualization
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chartview.cpp \
    main.cpp \
    mainwindow.cpp \
    math_utils.cpp \
    refrlogic1.cpp \
    scattergraph.cpp \
    scatterdatamodifier.cpp \
    settings.cpp \
    settingschartview.cpp \
    surfacegraph.cpp \
    surfacegraphmodifier.cpp \
    axesinputhandler.cpp

HEADERS += \
    chartview.h \
    mainwindow.h \
    math_utils.h \
    refrlogic1.h \
    scattergraph.h \
    scatterdatamodifier.h \
    settings.h \
    settingschartview.h \
    surfacegraph.h \
    surfacegraphmodifier.h \
    axesinputhandler.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Refractograms_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

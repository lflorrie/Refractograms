QT += core gui charts
QT += datavisualization
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += C:/Programs2/Boost/

SOURCES += \
    src/callout.cpp \
    src/chartview.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/math_utils.cpp \
    src/refrcharts.cpp \
    src/refrlogic1.cpp \
    src/refrworker.cpp \
    src/scatter3d.cpp \
    src/settings.cpp \
    src/settingschartview.cpp

HEADERS += \
    include/callout.h \
    include/chartview.h \
    include/mainwindow.h \
    include/math_utils.h \
    include/refrcharts.h \
    include/refrlogic1.h \
    include/refrtypes.h \
    include/refrworker.h \
    include/scatter3d.h \
    include/settings.h \
    include/settingschartview.h

FORMS += \
    mainwindow.ui \
    settingschartview.ui

TRANSLATIONS += \
    Refractograms_ru_RU.ts

RESOURCES += \
        resources.qrc


CONFIG += lrelease
CONFIG += embed_translations
INCLUDEPATH += include
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# deploy
QT_INSTALL_FRAMEWORK_PATH = C:/Qt/Tools/QtInstallerFramework/4.8

DESTDIR = $$PWD/../Refractograms/packages/ics.component/data

INPUT = $$PWD/config/config.xml $$PWD/packages
INSTALLER = refractograms-installer
refractograms-installer.input = INPUT
refractograms-installer.output = $$INSTALLER
refractograms-installer.CONFIG += target_predeps no_link combine
refractograms-installer.commands = $$QT_INSTALL_FRAMEWORK_PATH/bin/binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages ../Refractograms-Installer/${QMAKE_FILE_OUT}
QMAKE_EXTRA_COMPILERS += refractograms-installer

DESTDIR = $$PWD/../Refractograms/packages/ics.component/data
create_package.commands = $$quote(windeployqt --qmldir $$PWD/../Refractograms-Installer $${DESTDIR})
QMAKE_EXTRA_TARGETS += create_package
PRE_TARGETDEPS += create_package



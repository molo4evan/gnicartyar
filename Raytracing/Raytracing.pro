#-------------------------------------------------
#
# Project created by QtCreator 2019-05-23T09:30:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Raytracing
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    gui/mainwindow.cpp \
    model/primitives/primitive.cpp \
    model/primitives/sphere.cpp \
    model/primitives/triangle.cpp \
    model/primitives/quadrangle.cpp \
    model/primitives/box.cpp \
    model/primitives/plane.cpp \
    model/camera.cpp \
    model/scene.cpp \
    structures/luminocity.cpp \
    utils/paintingutils.cpp \
    gui/imagepainter.cpp \
    utils/renderthread.cpp \
    gui/rendersettingsdialog.cpp \
    gui/about.cpp

HEADERS += \
    gui/mainwindow.h \
    model/primitives/primitive.h \
    structures/reflection_params.h \
    structures/intersection_data.h \
    structures/beam.h \
    model/primitives/sphere.h \
    model/primitives/triangle.h \
    model/primitives/quadrangle.h \
    model/primitives/box.h \
    model/primitives/plane.h \
    model/camera.h \
    structures/draw_segment.h \
    structures/viewport_params.h \
    structures/gabarite_box.h \
    structures/illuminant.h \
    model/scene.h \
    structures/luminocity.h \
    structures/quality.h \
    utils/paintingutils.h \
    gui/imagepainter.h \
    utils/renderthread.h \
    gui/rendersettingsdialog.h \
    gui/about.h

FORMS += \
    gui/mainwindow.ui \
    gui/imagepainter.ui \
    gui/rendersettingsdialog.ui \
    gui/about.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QPainter>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

#include "about.h"
#include "rendersettingsdialog.h"

#include <model/camera.h>
#include <utils/paintingutils.h>
#include <utils/renderthread.h>
#include <model/primitives/sphere.h>
#include <model/primitives/box.h>
#include <model/primitives/triangle.h>
#include <model/primitives/quadrangle.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_actionRender_toggled(bool arg1);

    void on_actionSelect_view_toggled(bool arg1);

private:
    Ui::MainWindow *ui;

    static const QString folder;

    Camera camera;
    bool isRendering = false, isRendered = false, isCtrl = false;

    QImage wireframe, rendered;

    QLabel *percents;
    RenderThread *t;
    QPoint from;

    bool prog_toggle = false;

    QTimer *timerix, *timeriy, *timerdx, *timerdy;

    int getUbyX(double x);
    int getVbyY(double y);

    void showWireframe(vector<draw_segment> segments);
    void startRender();

    QString readLineWithoutComments(QTextStream *in);
    vector<double> parseDoublesFromStrig(QString str);
    QColor parseColorFromString(QString str);
    int parseIntFromString(QString str);
    void parseParamsFromString(QString str, QVector3D &point, QColor &color);


private slots:
    void incX();
    void decX();
    void incY();
    void decY();

    void callback();

    void on_actionOpen_scene_file_triggered();
    void on_actionOpen_render_settings_triggered();
    void on_actionInit_camera_triggered();

    void on_actionSave_image_triggered();
    void on_actionSave_render_settings_triggered();
    void on_actionRender_settings_triggered();
    void on_actionAbout_triggered();
};

#endif // MAINWINDOW_H

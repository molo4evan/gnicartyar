#ifndef CAMERA_H
#define CAMERA_H

#include "structures/viewport_params.h"
#include "structures/draw_segment.h"
#include "structures/quality.h"

#include "scene.h"

#include <QMatrix4x4>
#include <QVector2D>
#include <QImage>

using namespace std;

class Camera
{
public:
    Camera();
    void setCamParams(QVector3D pcam, QVector3D pview, QVector3D vup);
    vector<QVector3D> getCamParams();
    QMatrix4x4 getMcam();


    void setViewport(viewport_params params);
    viewport_params getViewport();
    QMatrix4x4 getMproj();

    void setQuality(quality q);
    quality getQuality();
    void setGamma(double g);
    double getGamma();

    void init();
    void scaleFocus(double coefficient);
    void moveForward(float delta);
    void moveUp(float delta);
    void moveRight(float delta);
    void rotate(double x_angle, double y_angle);
    void setAspectRatio(double ratio);

    vector<draw_segment> renderWireframe();
    QVector2D getCenter();
    vector<draw_segment> getIlluminants();

    Scene& getScene();

    void raytrace(QImage& img, std::function<void(size_t)> notify, std::function<void()> callback);

private:
    QMatrix4x4 mcam, mproj, mcam_inv, mview, mview_inv;

    QVector3D pcam{ -3, 0, 0 };
    QVector3D pview{ 0, 0, 0 };
    QVector3D vup{ 0, 0, 1 };
    viewport_params viewport;

    QColor x_color = Qt::red;
    QColor y_color = Qt::green;
    QColor z_color = Qt::blue;
    QColor default_color = Qt::black;

    Scene scene;
    quality qual = normal;
    double gamma = 1;
    double currentRatio = 1;

    void generateMcam();
    void generateMproj();

    pair<QVector4D, QVector4D> clip(QVector4D start, QVector4D end, bool &ok);
    QVector3D getNearClipPoint(double x, double y);
    void correct(vector<vector<Luminocity>> &lums);
};

#endif // CAMERA_H

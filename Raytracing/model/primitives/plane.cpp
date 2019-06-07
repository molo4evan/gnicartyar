#include "plane.h"

Plane::Plane(QVector3D n, double d)
{
    normal = n;
    this->d = d;
}

pair<QVector3D, double> Plane::getCoefs(){
    return {normal, d};
}

bool Plane::isShadowing(QVector3D from, QVector3D to){
    QVector3D dir = (to - from).normalized();
    double vd = QVector3D::dotProduct(normal, dir);
    if (vd == 0) return false;
    double v0 = -QVector3D::dotProduct(normal, from) - d;
    double t = v0 / vd;
    if (t < 0) return false;
    QVector3D point = from + t*dir;
    ip = point;
    return (point - from).length() < (to - from).length();
}

QVector3D Plane::getLastIntersectionPoint(){
    return ip;
}

intersection_data Plane::intersect(beam &beam, bool &isIntersected){
    intersection_data out;
    out.diff_refl = diff_refl;
    out.mirror_refl = mirror_refl;
    out.blinn_power = blinn_power;
    double vd = QVector3D::dotProduct(normal, beam.dir);
    if (vd >= 0){
        isIntersected = false;
        return out;
    }
    double v0 = -QVector3D::dotProduct(normal, beam.start) - d;
    double t = v0 / vd;
    if (t < 0){
        isIntersected = false;
    } else {
        isIntersected = true;
        out.point = beam.start + beam.dir * t;
        ip = out.point;
        out.normal = normal;
    }
    return out;
}

vector<pair<QVector3D, QVector3D>> Plane::getSegments(){
    return vector<pair<QVector3D, QVector3D>>();
}

gabarite_box Plane::getGabariteBox(){
    gabarite_box box;
    box.x_min = box.y_min = box.z_min = -std::numeric_limits<double>::infinity();
    box.x_max = box.y_max = box.z_max = std::numeric_limits<double>::infinity();
    return box;
}

#include "plane.h"
#include "triangle.h"

Triangle::Triangle(QVector3D p1, QVector3D p2, QVector3D p3){
    point1 = p1;
    point2 = p2;
    point3 = p3;
    box.x_max = max({point1.x(), point2.x(), point3.x()});
    box.x_min = min({point1.x(), point2.x(), point3.x()});
    box.y_max = max({point1.y(), point2.y(), point3.y()});
    box.y_min = min({point1.y(), point2.y(), point3.y()});
    box.z_max = max({point1.z(), point2.z(), point3.z()});
    box.z_min = min({point1.z(), point2.z(), point3.z()});

    QVector3D a = point3 - point1;
    QVector3D b = point2 - point1;
    QVector3D normal = QVector3D::crossProduct(b, a);
    areaabc = normal.length();
    normal.normalize();
    double d = -QVector3D::dotProduct(point1, normal);
    p = new Plane(normal, d);
}

Triangle::~Triangle(){
    if (p != nullptr) delete p;
}

vector<QVector3D> Triangle::getPoints(){
    return {point1, point2, point3};
}

bool Triangle::checkBaricentric(QVector3D p){
    QVector3D pb = p - point2;
    QVector3D pc = p - point3;
    QVector3D pa = p - point1;
    double areapab = QVector3D::crossProduct(pa, pb).length();
    double areapbc = QVector3D::crossProduct(pb, pc).length();
    double areapac = QVector3D::crossProduct(pa, pc).length();
    double alpha = abs(areapbc / areaabc);
    double beta = abs(areapac / areaabc);
    double gamma = abs(areapab / areaabc);
    return alpha <= 1 && beta <= 1 && gamma <= 1 && abs(1 - (alpha + beta + gamma)) < 0.001;
}

bool Triangle::isShadowing(QVector3D from, QVector3D to){
    return p->isShadowing(from, to) &&
            checkBaricentric(p->getLastIntersectionPoint());
}

intersection_data Triangle::intersect(beam &beam, bool &isIntersected){
    intersection_data out = p->intersect(beam, isIntersected);
    if (isIntersected) {
        isIntersected = checkBaricentric(out.point);
    }
    out.diff_refl = diff_refl;
    out.mirror_refl = mirror_refl;
    out.blinn_power = blinn_power;
    return out;
}

vector<pair<QVector3D, QVector3D>> Triangle::getSegments(){
    return {
        {point1, point2},
        {point2, point3},
        {point3, point1}
    };
}

gabarite_box Triangle::getGabariteBox(){
    return box;
}

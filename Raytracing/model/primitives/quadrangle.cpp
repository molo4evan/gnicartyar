#include "quadrangle.h"

Quadrangle::Quadrangle(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4):
    point1(p1), point2(p2), point3(p3), point4(p4), t1(p1, p2, p3), t2(p3, p4, p1)
{
    box.x_max = max({point1.x(), point2.x(), point3.x(), point4.x()});
    box.x_min = min({point1.x(), point2.x(), point3.x(), point4.x()});
    box.y_max = max({point1.y(), point2.y(), point3.y(), point4.y()});
    box.y_min = min({point1.y(), point2.y(), point3.y(), point4.y()});
    box.z_max = max({point1.z(), point2.z(), point3.z(), point4.z()});
    box.z_min = min({point1.z(), point2.z(), point3.z(), point4.z()});
}

vector<QVector3D> Quadrangle::getPoints(){
    return {point1, point2, point3, point4};
}

bool Quadrangle::isShadowing(QVector3D from, QVector3D to){
    return t1.isShadowing(from, to) || t2.isShadowing(from, to);
}

intersection_data Quadrangle::intersect(beam &beam, bool &isIntersected){
    bool ii1;
    intersection_data out = t1.intersect(beam, ii1);
    if (ii1){
        isIntersected = ii1;
        out.diff_refl = diff_refl;
        out.mirror_refl = mirror_refl;
        out.blinn_power = blinn_power;
        return out;
    } else {
        out = t2.intersect(beam, isIntersected);
        out.diff_refl = diff_refl;
        out.mirror_refl = mirror_refl;
        out.blinn_power = blinn_power;
        return out;
    }
}

vector<pair<QVector3D, QVector3D>> Quadrangle::getSegments(){
    return {
        {point1, point2},
        {point2, point3},
        {point3, point4},
        {point4, point1}
    };
}

gabarite_box Quadrangle::getGabariteBox(){
    return box;
}

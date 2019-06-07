#include "box.h"

Box::Box(QVector3D min, QVector3D max)
{
    minPoint = min;
    maxPoint = max;
}

Box::Box(gabarite_box box):
    minPoint(box.x_min, box.y_min, box.z_min),
    maxPoint(box.x_max, box.y_max, box.z_max){}

pair<QVector3D, QVector3D> Box::getMinMax(){
    return pair<QVector3D, QVector3D>(minPoint, maxPoint);
}

bool Box::isShadowing(QVector3D from, QVector3D to){
    beam b;
    b.dir = (to - from).normalized();
    b.start = from;
    bool ii;
    intersect(b, ii);
    return ii;
}

intersection_data Box::intersect(beam &beam, bool &isIntersected){
    intersection_data out;
    out.diff_refl = diff_refl;
    out.mirror_refl = mirror_refl;
    out.blinn_power = blinn_power;
    double tfar = numeric_limits<double>::infinity(), tnear = -numeric_limits<double>::infinity();
    if (beam.dir.x() != 0){
        double t1 = (minPoint.x() - beam.start.x()) / beam.dir.x();
        double t2 = (maxPoint.x() - beam.start.x()) / beam.dir.x();
        tnear = max(tnear, min(t1, t2));
        tfar = min(tfar, max(t1, t2));
    }
    if (beam.dir.y() != 0){
        double t1 = (minPoint.y() - beam.start.y()) / beam.dir.y();
        double t2 = (maxPoint.y() - beam.start.y()) / beam.dir.y();
        tnear = max(tnear, min(t1, t2));
        tfar = min(tfar, max(t1, t2));
    }
    if (beam.dir.z() != 0){
        double t1 = (minPoint.z() - beam.start.z()) / beam.dir.z();
        double t2 = (maxPoint.z() - beam.start.z()) / beam.dir.z();
        tnear = max(tnear, min(t1, t2));
        tfar = min(tfar, max(t1, t2));
    }
    isIntersected = tnear <= tfar && tfar >= 0;
    return out;
}

vector<pair<QVector3D, QVector3D>> Box::getSegments(){
    vector<pair<QVector3D, QVector3D>> out = {
        {QVector3D(minPoint.x(), minPoint.y(), minPoint.z()), QVector3D(maxPoint.x(), minPoint.y(), minPoint.z())},
        {QVector3D(minPoint.x(), minPoint.y(), minPoint.z()), QVector3D(minPoint.x(), maxPoint.y(), minPoint.z())},
        {QVector3D(minPoint.x(), minPoint.y(), minPoint.z()), QVector3D(minPoint.x(), minPoint.y(), maxPoint.z())},

        {QVector3D(maxPoint.x(), maxPoint.y(), maxPoint.z()), QVector3D(minPoint.x(), maxPoint.y(), maxPoint.z())},
        {QVector3D(maxPoint.x(), maxPoint.y(), maxPoint.z()), QVector3D(maxPoint.x(), minPoint.y(), maxPoint.z())},
        {QVector3D(maxPoint.x(), maxPoint.y(), maxPoint.z()), QVector3D(maxPoint.x(), maxPoint.y(), minPoint.z())},

        {QVector3D(maxPoint.x(), minPoint.y(), minPoint.z()), QVector3D(maxPoint.x(), maxPoint.y(), minPoint.z())},
        {QVector3D(maxPoint.x(), minPoint.y(), minPoint.z()), QVector3D(maxPoint.x(), minPoint.y(), maxPoint.z())},
        {QVector3D(minPoint.x(), maxPoint.y(), minPoint.z()), QVector3D(maxPoint.x(), maxPoint.y(), minPoint.z())},
        {QVector3D(minPoint.x(), maxPoint.y(), minPoint.z()), QVector3D(minPoint.x(), maxPoint.y(), maxPoint.z())},
        {QVector3D(minPoint.x(), minPoint.y(), maxPoint.z()), QVector3D(maxPoint.x(), minPoint.y(), maxPoint.z())},
        {QVector3D(minPoint.x(), minPoint.y(), maxPoint.z()), QVector3D(minPoint.x(), maxPoint.y(), maxPoint.z())}
    };
    return out;
}

gabarite_box Box::getGabariteBox(){
    gabarite_box box;
    box.x_min = minPoint.x();
    box.y_min = minPoint.y();
    box.z_min = minPoint.z();
    box.x_max = maxPoint.x();
    box.y_max = maxPoint.y();
    box.z_max = maxPoint.z();
    return box;
}

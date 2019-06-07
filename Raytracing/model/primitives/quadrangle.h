#ifndef QUADRANGLE_H
#define QUADRANGLE_H

#include "primitive.h"
#include "triangle.h"

class Quadrangle: public Primitive
{
public:
    Quadrangle(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4);
    vector<QVector3D> getPoints();

    virtual bool isShadowing(QVector3D from, QVector3D to);
    virtual intersection_data intersect(beam &beam, bool &isIntersected);
    virtual vector<pair<QVector3D, QVector3D>> getSegments();
    virtual gabarite_box getGabariteBox();

private:
    QVector3D point1;
    QVector3D point2;
    QVector3D point3;
    QVector3D point4;

    gabarite_box box;

    Triangle t1, t2;
};

#endif // QUADRANGLE_H

#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"

class Sphere: public Primitive
{
public:
    Sphere(QVector3D center, double radius);
    double getRadius();
    QVector3D getCenter();

    virtual intersection_data intersect(beam &beam, bool &isIntersected);
    virtual bool isShadowing(QVector3D from, QVector3D to);
    virtual vector<pair<QVector3D, QVector3D>> getSegments();
    virtual gabarite_box getGabariteBox();

private:
    QVector3D center;   //s0
    double radius;
    double grid = 10;

    gabarite_box b;

    QVector3D getPoint(double phi, double psi);
};

#endif // SPHERE_H

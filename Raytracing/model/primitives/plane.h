#ifndef PLANE_H
#define PLANE_H

#include "primitive.h"

class Plane: public Primitive
{
public:
    Plane(QVector3D normal, double d);
    pair<QVector3D, double> getCoefs();

    virtual bool isShadowing(QVector3D from, QVector3D to);
    QVector3D getLastIntersectionPoint();
    virtual intersection_data intersect(beam &beam, bool &isIntersected);
    virtual vector<pair<QVector3D, QVector3D>> getSegments();
    virtual gabarite_box getGabariteBox();
private:
    QVector3D normal;
    double d;

    QVector3D ip;
};

#endif // PLANE_H

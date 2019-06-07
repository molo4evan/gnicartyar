#ifndef BOX_H
#define BOX_H

#include "primitive.h"

class Box: public Primitive
{
public:
    Box(QVector3D min, QVector3D max);
    Box(gabarite_box box);
    pair<QVector3D, QVector3D> getMinMax();

    virtual bool isShadowing(QVector3D from, QVector3D to);
    virtual intersection_data intersect(beam &beam, bool &isIntersected);   //return is invalid
    virtual vector<pair<QVector3D, QVector3D>> getSegments();
    virtual gabarite_box getGabariteBox();

private:
    QVector3D minPoint;
    QVector3D maxPoint;
};

#endif // BOX_H

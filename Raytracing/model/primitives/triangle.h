#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "plane.h"
#include "primitive.h"

class Triangle: public Primitive
{
public:
    Triangle(QVector3D p1, QVector3D p2, QVector3D p3);
    ~Triangle();
    vector<QVector3D> getPoints();

    virtual bool isShadowing(QVector3D from, QVector3D to);
    virtual intersection_data intersect(beam &beam, bool &isIntersected);
    virtual vector<pair<QVector3D, QVector3D>> getSegments();
    virtual gabarite_box getGabariteBox();

private:
    QVector3D point1;
    QVector3D point2;
    QVector3D point3;

    gabarite_box box;
    Plane *p = nullptr;
    double areaabc;

    bool checkBaricentric(QVector3D p);
};

#endif // TRIANGLE_H

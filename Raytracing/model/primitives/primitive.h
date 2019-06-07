#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <structures/beam.h>
#include <structures/gabarite_box.h>
#include <structures/intersection_data.h>
#include <structures/reflection_params.h>
#include <cmath>
#include <QDebug>

using namespace std;

class Primitive
{
public:
    Primitive();
    virtual ~Primitive();
    void setOpticCharacteristics(reflection_params diff, reflection_params mirror, double pow);
    reflection_params getDiffReflection();
    reflection_params getMirrorReflection();
    double getBlinnPower();

    virtual intersection_data intersect(beam &beam, bool &isIntersected) = 0;
    virtual bool isShadowing(QVector3D from, QVector3D to) = 0;
    virtual vector<pair<QVector3D, QVector3D>> getSegments() = 0;
    virtual gabarite_box getGabariteBox() = 0;

protected:
    reflection_params diff_refl;    // ka, kd
    reflection_params mirror_refl;  // ks
    double blinn_power;
};

#endif // PRIMITIVE_H

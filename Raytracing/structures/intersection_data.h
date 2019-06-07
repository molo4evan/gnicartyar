#ifndef INTERSECTION_DATA_H
#define INTERSECTION_DATA_H

#include "reflection_params.h"
#include <QVector3D>

typedef struct intersection_data {
    QVector3D point;
    QVector3D normal;
    reflection_params diff_refl;    // ka, kd
    reflection_params mirror_refl;  // ks
    double blinn_power;
} intersection_data;
#endif // INTERSECTION_DATA_H

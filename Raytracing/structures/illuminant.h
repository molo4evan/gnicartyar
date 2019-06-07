#ifndef ILLUMINANT_H
#define ILLUMINANT_H

#include "luminocity.h"

#include <QColor>
#include <QVector3D>

typedef struct illuminant {
    QVector3D pos;
    Luminocity luminocity;
} illuminant;

#endif // ILLUMINANT_H

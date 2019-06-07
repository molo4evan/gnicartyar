#ifndef BEAM_H
#define BEAM_H

#include <QVector3D>

typedef struct beam {
    QVector3D start;    //r0
    QVector3D dir;      //rd
} beam;
#endif // BEAM_H

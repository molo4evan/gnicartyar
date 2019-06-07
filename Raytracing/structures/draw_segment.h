#ifndef DRAW_SEGMENT_H
#define DRAW_SEGMENT_H

#include <QColor>
#include <QVector2D>

typedef struct draw_segment {
    QVector2D start;
    QVector2D end;
    QColor color;
} draw_segment;
#endif // DRAW_SEGMENT_H

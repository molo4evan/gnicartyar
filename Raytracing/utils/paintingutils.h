#ifndef PAINTINGUTILS_H
#define PAINTINGUTILS_H

#include <QImage>
#include <QPainter>

class PaintingUtils
{
public:
    static void drawLine(QImage& img, int x0, int y0, int x1, int y1, const QColor& color);
    static void setPixel(QImage& img, int x, int y, const QColor& color);
    static void drawCircle(QImage& img, int x, int y, int r, const QColor& color);
};

#endif // PAINTINGUTILS_H

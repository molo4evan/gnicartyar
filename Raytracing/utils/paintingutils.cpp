#include "paintingutils.h"

void PaintingUtils::drawLine(QImage &img, int x0, int y0, int x1, int y1, const QColor &color){
    bool swapped = false;
    int x = x0, y = y0;
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int shift_x = x1 - x0 > 0 ? 1 : (x1 - x0 < 0 ? -1 : 0);
    int shift_y = y1 - y0 > 0 ? 1 : (y1 - y0 < 0 ? -1 : 0);
    if (dy > dx){
        int tmp = dx;
        dx = dy;
        dy = tmp;
        swapped = true;
    }
    int err = -dx;
    for (int i = 0; i <= dx; ++i){
        setPixel(img, x, y, color);
        if (swapped){
            y += shift_y;
        } else {
            x += shift_x;
        }
        err += 2*dy;
        if (err > 0){
            err -= 2*dx;
            if (swapped){
                x += shift_x;
            } else {
                y += shift_y;
            }
        }
    }
}

void PaintingUtils::drawCircle(QImage &img, int x, int y, int r, const QColor &color){
    QPainter painter(&img);
    painter.setPen(Qt::black);
    painter.setBrush(color);
    QRect rect(x - r, y - r, r * 2, r * 2);
    painter.drawEllipse(rect);
}

void PaintingUtils::setPixel(QImage &img, int x, int y, const QColor &color){
    if (x < 0 || y < 0 || x >= img.width() || y >= img.height()) return;
    uchar* data = img.bits();
    data[y * img.bytesPerLine() + x * 3] = static_cast<uchar>(color.red());
    data[y * img.bytesPerLine() + x * 3 + 1] = static_cast<uchar>(color.green());
    data[y * img.bytesPerLine() + x * 3 + 2] = static_cast<uchar>(color.blue());
}

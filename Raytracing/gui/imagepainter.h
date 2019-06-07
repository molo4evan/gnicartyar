#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H

#include <QWidget>
#include <QPainter>
#include <QDebug>

namespace Ui {
class ImagePainter;
}

class ImagePainter : public QWidget
{
    Q_OBJECT

public:
    explicit ImagePainter(QWidget *parent = nullptr);
    ~ImagePainter();
    void setImage(QImage img);

protected slots:
    void paintEvent(QPaintEvent *);

private:
    Ui::ImagePainter *ui;

    QImage img;
};

#endif // IMAGEPAINTER_H

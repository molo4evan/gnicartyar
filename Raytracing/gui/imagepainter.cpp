#include "imagepainter.h"
#include "ui_imagepainter.h"

ImagePainter::ImagePainter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagePainter)
{
    ui->setupUi(this);
}

ImagePainter::~ImagePainter()
{
    delete ui;
}

void ImagePainter::setImage(QImage img){
    this->img = img;
    repaint();
}

void ImagePainter::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawImage(0, 0, img);
}

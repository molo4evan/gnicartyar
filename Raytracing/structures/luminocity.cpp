#include "luminocity.h"

Luminocity::Luminocity(double ir, double ig, double ib):
    ir(ir), ig(ig), ib(ib){}

Luminocity::Luminocity(){}

Luminocity::Luminocity(const QColor& color){
    ir = color.red() / 255.0;
    ig = color.green() / 255.0;
    ib = color.blue() / 255.0;
}

Luminocity& Luminocity::operator+=(const Luminocity &other){
    ir += other.ir;
    ig += other.ig;
    ib += other.ib;
    return *this;
}

Luminocity& Luminocity::operator/=(const double coef){
    ir /= coef;
    ig /= coef;
    ib /= coef;
    return *this;
}

Luminocity::operator QColor(){
    return QColor(
        static_cast<int>(255 * ir),
        static_cast<int>(255 * ig),
        static_cast<int>(255 * ib)
    );
}

double Luminocity::Ir(){
    return ir;
}

double Luminocity::Ig(){
    return ig;
}

double Luminocity::Ib(){
    return ib;
}

void Luminocity::setIr(double i){
    ir = i;
}

void Luminocity::setIg(double i){
    ig = i;
}

void Luminocity::setIb(double i){
    ib = i;
}

const Luminocity operator+(const Luminocity &left, const Luminocity &right){
    return Luminocity(left.ir + right.ir, left.ig + right.ig, left.ib + right.ib);
}

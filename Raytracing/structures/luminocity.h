#ifndef LUMINOCITY_H
#define LUMINOCITY_H

#include <QColor>
#include <initializer_list>

using namespace std;

class Luminocity{
public:
    Luminocity(double ir, double ig, double ib);
    Luminocity(const QColor& color);
    Luminocity();

    Luminocity& operator+=(const Luminocity &other);
    Luminocity& operator /=(const double coef);
    friend const Luminocity operator+(const Luminocity &left, const Luminocity &right);

    operator QColor();

    double Ir();
    double Ig();
    double Ib();

    void setIr(double i);
    void setIg(double i);
    void setIb(double i);

private:
    double ir = 0;
    double ig = 0;
    double ib = 0;
};

#endif // LUMINOCITY_H

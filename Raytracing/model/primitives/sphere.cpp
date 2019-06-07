#include "sphere.h"

Sphere::Sphere(QVector3D center, double radius){
    this->center = center;
    this->radius = radius;
    b.x_min = center.x() - radius;
    b.x_max = center.x() + radius;
    b.y_min = center.y() - radius;
    b.y_max = center.y() + radius;
    b.z_min = center.z() - radius;
    b.z_max = center.z() + radius;
}

double Sphere::getRadius(){
    return radius;
}

QVector3D Sphere::getCenter(){
    return center;
}

bool Sphere::isShadowing(QVector3D from, QVector3D to){
    QVector3D dir = (to - from).normalized();
    QVector3D dist = from - center;
    double b = QVector3D::dotProduct(dir, dist);
    double c = dist.lengthSquared() - radius*radius;
    double d = b*b - c;
    if (d < 0) return false;
    double t;
    double t0 = -b - sqrt(d);
    if (t0 < 0){
        double t1 = -b + sqrt(d);
        if (t1 < 0){
            return false;
        } else {
            t = t1;
        }
    }
    QVector3D point = from + t*dir;
    return (point - from).length() < (to - from).length();
}

intersection_data Sphere::intersect(beam &beam, bool &isIntersected){
    intersection_data out;
    out.diff_refl = diff_refl;
    out.mirror_refl = mirror_refl;
    out.blinn_power = blinn_power;
    QVector3D dist = beam.start - center;
    double b = QVector3D::dotProduct(beam.dir, dist);
    double c = dist.lengthSquared() - radius*radius;
    double d = b*b - c;
    if (d < 0){
        isIntersected = false;
        return out;
    } else {
        double t;
        double t0 = -b - sqrt(d);
        if (t0 < 0){
            isIntersected = false;
            return out;
        } else {
            t = t0;
        }
        out.point = beam.start + t*beam.dir;
        out.normal = (out.point - center) / radius;
        isIntersected = true;
        return out;
    }
}

QVector3D Sphere::getPoint(double phi, double psi){
    double sinphi = sin(phi), cosphi = cos(phi),
            sinpsi = sin(psi), cospsi = cos(psi);
    return QVector3D(
                center.x() + radius * sinphi * cospsi,
                center.y() + radius * sinphi * sinpsi,
                center.z() + radius * cosphi);
}

vector<pair<QVector3D, QVector3D>> Sphere::getSegments(){
    double phistep = M_PI / grid, psistep = phistep * 2;
    vector<pair<QVector3D, QVector3D>> out;
    for (int i = 0; i <= grid; ++i){
        QVector3D start = getPoint(i * phistep, 0);
        for (int j = 0; j <= grid; ++j) {
            QVector3D end = getPoint((i + 1) * phistep, j * psistep);
            out.push_back({start, end});
            end = getPoint(i * phistep, (j + 1) * psistep);
            out.push_back({start, end});
            start = end;
        }
    }
    return out;
}

gabarite_box Sphere::getGabariteBox(){
    return b;
}


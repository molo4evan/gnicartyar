#include "scene.h"

#include <model/primitives/box.h>

Scene::Scene()
{

}

Luminocity Scene::getFoneLuminocity(){
    return fone;
}

Luminocity Scene::getAmbient(){
    return ambient;
}

void Scene::addPrimitive(shared_ptr<Primitive> p){
    primitives.push_back(p);
}

void Scene::clearPrimitives(){
    primitives.clear();
}

const vector<shared_ptr<Primitive>>& Scene::getPrimitives(){
    return primitives;
}

void Scene::addIlluminant(illuminant i){
    illuminants.push_back(i);
}

void Scene::clearIlluminants(){
    illuminants.clear();
}

vector<illuminant> Scene::getIlluminants(){
    return illuminants;
}

void Scene::setFoneLuminocity(Luminocity l){
    fone = l;
}

void Scene::setAmbient(Luminocity l){
    ambient = l;
}

gabarite_box Scene::getGabariteBox(){
    gabarite_box out;
    if (primitives.empty()){
        out.x_max = out.x_min = out.y_max = out.y_min = out.z_max = out.z_min = 0;
        return out;
    }
    for (size_t i = 0; i < primitives.size(); ++i) {
        gabarite_box another = primitives[i]->getGabariteBox();
        out.x_min = min(out.x_min, another.x_min);
        out.x_max = max(out.x_max, another.x_max);
        out.y_min = min(out.y_min, another.y_min);
        out.y_max = max(out.y_max, another.y_max);
        out.z_min = min(out.z_min, another.z_min);
        out.z_max = max(out.z_max, another.z_max);
    }
    double percent = 0.05;
    double delta_x = out.x_max - out.x_min;
    out.x_max += delta_x * percent / 2;
    out.x_min -= delta_x * percent / 2;
    double delta_y = out.y_max - out.y_min;
    out.y_max += delta_y * percent / 2;
    out.y_min -= delta_y * percent / 2;
    double delta_z = out.z_max - out.z_min;
    out.z_max += delta_z * percent / 2;
    out.z_min -= delta_z * percent / 2;
    return out;
}

void Scene::setDepth(size_t d){
    depth = d;
}

size_t Scene::getDepth(){
    return depth;
}

void Scene::setEpoint(QVector3D e){
    Epoint = e;
}

QVector3D Scene::findE(QVector3D &point){
    return (Epoint - point).normalized();
}

QVector3D Scene::findH(QVector3D &L, QVector3D &point){
    QVector3D LE = L + findE(point);
    return LE.normalized();
}

intersection_data Scene::findIntersection(beam &b, bool &isIntersected){
    vector<shared_ptr<Primitive>> box_intersected;
    vector<intersection_data> intersected;
    intersection_data out;
    bool isInt = false;
    bool pi;
    for (auto &p: primitives) {
        Box box(p->getGabariteBox());
        box.intersect(b, pi);
        if (pi) box_intersected.push_back(p);
    }
    for (auto &p: box_intersected){
        intersection_data inter = p->intersect(b, pi);
        if (pi) intersected.push_back(inter);
        isInt = isInt || pi;
    }
    if (!isInt || intersected.empty()){
        isIntersected = false;
        return out;
    } else {
        out = intersected[0];
        double r2cur = (out.point - b.start).lengthSquared();
        for (size_t i = 1; i < intersected.size(); ++i){
            intersection_data inter = intersected[i];
            double r2 = (inter.point - b.start).lengthSquared();
            if (r2 < r2cur){
                out = inter;
                r2cur = r2;
            }
        }
        isIntersected = true;
        return out;
    }
}

bool Scene::isVisible(QVector3D &lum_pos, QVector3D &point){    //TODO: optimize
    beam b;
    b.start = lum_pos;
    b.dir = (point - lum_pos).normalized();
    bool i;
    for (auto &p: primitives) {
        Box box(p->getGabariteBox());
        box.intersect(b, i);
        if (!i) continue;
        if (p->isShadowing(lum_pos, point)) return false;
    }
    return true;
}

Luminocity Scene::getLuminocity(beam &beam, size_t depth){
    if (depth >= this->depth) throw "Incorrect depth";
    bool isIntersected;
    intersection_data intersection = findIntersection(beam, isIntersected);
    if (!isIntersected) return fone;
    else {
        Luminocity out(
            intersection.diff_refl.cr * ambient.Ir(),
            intersection.diff_refl.cg * ambient.Ig(),
            intersection.diff_refl.cb * ambient.Ib()
        );
        for (auto& illuminant: illuminants) {
            QVector3D uppoint = intersection.point + intersection.normal * 0.0001;
            if (isVisible(uppoint, illuminant.pos)){
                QVector3D L = (illuminant.pos - intersection.point);
                double d = L.length();
                L.normalize();
                QVector3D H = findH(L, intersection.point).normalized();
                double nl = QVector3D::dotProduct(intersection.normal, L);
                double nh = QVector3D::dotProduct(intersection.normal, H);
                if (nl < 0 || nh < 0) continue;
                nh = pow(nh, intersection.blinn_power);
                double fatt = 1 / (1 + d);
                Luminocity lum = illuminant.luminocity;
                lum.setIr(lum.Ir() * fatt * (intersection.diff_refl.cr * nl + intersection.mirror_refl.cr * nh));
                lum.setIg(lum.Ig() * fatt * (intersection.diff_refl.cg * nl + intersection.mirror_refl.cg * nh));
                lum.setIb(lum.Ib() * fatt * (intersection.diff_refl.cb * nl + intersection.mirror_refl.cb * nh));
                out += lum;
            } else {

            }
        }
        if (depth < this->depth - 1){
            QVector3D L = -beam.dir;
            QVector3D N = intersection.normal;
            QVector3D R = 2 * QVector3D::dotProduct(N, L) * N - L;
            struct beam reflected = {intersection.point, R};
            Luminocity refLum = getLuminocity(reflected, depth + 1);
            refLum.setIr(refLum.Ir() * intersection.mirror_refl.cr);
            refLum.setIg(refLum.Ig() * intersection.mirror_refl.cg);
            refLum.setIb(refLum.Ib() * intersection.mirror_refl.cb);
            out += refLum;
        }
        return out;
    }
}

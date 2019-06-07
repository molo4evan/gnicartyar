#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <model/primitives/primitive.h>
#include <structures/illuminant.h>
#include <structures/luminocity.h>

class Scene
{
public:
    Scene();

    void addPrimitive(shared_ptr<Primitive> p);
    void clearPrimitives();
    const vector<shared_ptr<Primitive>>& getPrimitives();

    void addIlluminant(illuminant i);
    void clearIlluminants();
    vector<illuminant> getIlluminants();

    void setFoneLuminocity(Luminocity l);
    Luminocity getFoneLuminocity();
    void setAmbient(Luminocity l);
    Luminocity getAmbient();

    gabarite_box getGabariteBox();

    void setDepth(size_t d);
    size_t getDepth();

    void setEpoint(QVector3D e);

    Luminocity getLuminocity(beam &beam, size_t depth);

private:
    vector<shared_ptr<Primitive>> primitives;
    vector<illuminant> illuminants;
    size_t depth = 3;
    QVector3D Epoint;
    Luminocity fone, ambient;

    QVector3D findE(QVector3D &point);
    QVector3D findH(QVector3D &L, QVector3D &point);
    bool isVisible(QVector3D &lum_pos, QVector3D &point);

    intersection_data findIntersection(beam &b, bool &isIntersected);
};

#endif // SCENE_H

#include "primitive.h"

Primitive::Primitive(){}

Primitive::~Primitive(){}

void Primitive::setOpticCharacteristics(reflection_params diff, reflection_params mirror, double pow){
    this->diff_refl = diff;
    this->mirror_refl = mirror;
    this->blinn_power = pow;
}

reflection_params Primitive::getDiffReflection(){
    return diff_refl;
}

reflection_params Primitive::getMirrorReflection(){
    return mirror_refl;
}

double Primitive::getBlinnPower(){
    return blinn_power;
}



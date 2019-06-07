#ifndef GABARITE_BOX_H
#define GABARITE_BOX_H

#include <limits>

typedef struct gabarite_box {
    double x_min = std::numeric_limits<double>::infinity();
    double x_max = -std::numeric_limits<double>::infinity();
    double y_min = std::numeric_limits<double>::infinity();
    double y_max = -std::numeric_limits<double>::infinity();
    double z_min = std::numeric_limits<double>::infinity();
    double z_max = -std::numeric_limits<double>::infinity();
} gabarite_box;

#endif // GABARITE_BOX_H

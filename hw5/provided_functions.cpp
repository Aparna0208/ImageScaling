// provided.  do not edit. do not submit.
#include <cstdint>
#include <cmath>
#include "functions.h"

// helper function for bicubic interpolation
double p(double x1, double x2, double x3, double x) {
    return (x - x1) * (x - x2) * (x - x3);
}

// performs bicubic interpolation on four color values
uint8_t bicubic_color(double t, uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4) {
    double x1 = -t - 1;
    double x2 = -t;
    double x3 = 1 - t;
    double x4 = 2 - t;

    double color = (p(x2, x3, x4, 0) * c1 / p(x2, x3, x4, x1)) +
                   (p(x1, x3, x4, 0) * c2 / p(x1, x3, x4, x2)) +
                   (p(x1, x2, x4, 0) * c3 / p(x1, x2, x4, x3)) +
                   (p(x1, x2, x3, 0) * c4 / p(x1, x2, x3, x4));

    if (color < 0)
        return 0;
    else if (color > 255)
        return 255;
    else
        return std::round(color);
}

Pixel bicubic_pixel(double t, const Pixel& p1, const Pixel& p2, const Pixel& p3, const Pixel& p4) {
    uint8_t red = bicubic_color(t, p1.red, p2.red, p3.red, p4.red);
    uint8_t green = bicubic_color(t, p1.green, p2.green, p3.green, p4.green);
    uint8_t blue = bicubic_color(t, p1.blue, p2.blue, p3.blue, p4.blue);
    return {red, green, blue};
}

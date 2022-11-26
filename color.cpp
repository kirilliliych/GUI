#include "color.hpp"


Color::Color()
  : red_f_  (1),
    green_f_(1),
    blue_f_ (1),
    alpha_f_(1)
{
    color_.r = COLOR_MAX_VALUE;
    color_.g = COLOR_MAX_VALUE;
    color_.b = COLOR_MAX_VALUE;
    color_.a = COLOR_MAX_VALUE;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    color_.r = r;
    color_.g = g;
    color_.b = b;
    color_.a = a;

    update_floats();
}

Color::Color(float r, float g, float b, float a)
{
    assert(std::isfinite(r));
    assert(std::isfinite(g));
    assert(std::isfinite(b));
    assert(std::isfinite(a));

    red_f_   = min(r, (float) 1);
    green_f_ = min(g, (float) 1);
    blue_f_  = min(b, (float) 1);
    alpha_f_ = min(a, (float) 1);

    red_f_   = max(red_f_,   (float) 0);
    green_f_ = max(green_f_, (float) 0);
    blue_f_  = max(blue_f_,  (float) 0);
    alpha_f_ = max(alpha_f_, (float) 0);

    update_ints();
}

Color::Color(unsigned rgba)
{
    color_.r = (uint8_t) ((rgba & 0xFF000000) >> 24);
    color_.g = (uint8_t) ((rgba & 0x00FF0000) >> 16);
    color_.b = (uint8_t) ((rgba & 0x0000FF00) >> 8);
    color_.a = (uint8_t)  (rgba & 0x000000FF);

    update_floats();
}


Vector3d color_to_vector(const Color &color)
{
    return Vector3d{color.get_red_f(), color.get_green_f(), color.get_blue_f()};
}

Vector3d color255_to_vector(const Color &color)
{
    return Vector3d{(float) color.get_r(), (float) color.get_g(), (float) color.get_b()};
}

Color vector_to_color(const Vector3d &vector)
{
    return Color{vector.get_x(), vector.get_y(), vector.get_z()};
}
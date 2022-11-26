#include "utilities.hpp"


bool is_in_rectangle(const Point3d *point, const Point3d *rect_angle1, const Point3d *rect_angle2)
{
    assert(point       != nullptr);
    assert(rect_angle1 != nullptr);
    assert(rect_angle2 != nullptr);

    return ((rect_angle1->get_x() - FLOAT_COMPARISON_PRECISION < point->get_x()) && (point->get_x() < rect_angle2->get_x() + FLOAT_COMPARISON_PRECISION) &&
            (rect_angle1->get_y() - FLOAT_COMPARISON_PRECISION < point->get_y()) && (point->get_y() < rect_angle2->get_y() + FLOAT_COMPARISON_PRECISION))
    ? true : false;
}

double get_square(double value)
{
    assert(std::isfinite(value));

    return value * value;
}

float get_square(float value)
{
    assert(std::isfinite(value));

    return value * value;
}

int get_square(int value)
{
    return value * value;
}

int min(int val1, int val2)
{
    return val1 > val2 ? val2 : val1;
}

int max(int val1, int val2)
{
    return val1 > val2 ? val1 : val2;
}

float min(float val1, float val2)
{
    return val1 - val2 > FLOAT_COMPARISON_PRECISION ? val2 : val1;
}

float max(float val1, float val2)
{
    return val1 - val2 > FLOAT_COMPARISON_PRECISION ? val1 : val2;
}

float get_random_fractional_float()
{
    return rand() / (RAND_MAX + 1.0);
}

float get_random_fractional_float(float min, float max)
{
    assert(std::isfinite(min));
    assert(std::isfinite(max));

    return min + abs(max - min) * get_random_fractional_float();
}

float degrees_to_radians(float degrees)
{
    assert(std::isfinite(degrees));

    return degrees * PI / 180;
}

float radians_to_degrees(float radians)
{
    assert(std::isfinite(radians));

    return radians * 180 / PI;
}
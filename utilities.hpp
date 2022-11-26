#ifndef UTILITIES_HPP
#define UTILITIES_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "point3d.hpp"


const float FLOAT_COMPARISON_PRECISION = 0.00001;
const float FLOAT_INFINITY = INFINITY;

const float EPSILON_DISTANCE = 0.001;
const float PI = 3.141593;

static const void *ABSENT_PTR_POISONED_VALUE = (const void *) 0xDEADBEEF;


bool is_in_rectangle(const Point3d *point, const Point3d *rect_angle1, const Point3d *rect_angle2);

double get_square(double value);

float get_square(float value);

int get_square(int value);

int min(int val1, int val2);

int max(int val1, int val2);

float min(float val1, float val2);

float max(float val1, float val2);

float get_random_fractional_float();

float get_random_fractional_float(float min, float max);

float degrees_to_radians(float degrees);

float radians_to_degrees(float radians);


#endif
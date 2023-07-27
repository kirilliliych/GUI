#ifndef RAY_HPP
#define RAY_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include "../geometry/vector3d.hpp"
#include "../utilities/color.hpp"
#include "../utilities/colorlibrary.hpp"
#include "sphere.hpp"


class Sphere;
struct SphereArr;
struct IntersectionData;


class Ray
{
public:
//-----------------------------------------------------------------------
    Ray(){};

    Ray(const Vector3d &origin, const Vector3d &direction)
      :  origin_   (origin),
         direction_(direction)
    {
        normalize_vector(&direction_);
    }
//-----------------------------------------------------------------------
    Vector3d get_origin() const
    {
        return origin_;
    }

    Vector3d get_direction() const
    {
        return direction_;
    }

    Vector3d get_position_at(float distance) const
    {
        return origin_ + direction_ * distance;
    }
//----------------------------Variables----------------------------------
    Vector3d origin_{};
    Vector3d direction_{};
};

bool ray_intersects_sphere(const Sphere &sphere, 
                           const Ray &ray, 
                           float dist_min, 
                           float dist_max, 
                           IntersectionData *data);

bool get_closest_intersection(const Ray &ray, const SphereArr &spheres, IntersectionData *data, Sphere *closest_sphere);

Color get_background_color(const Vector3d &normalized_ray_direction);

Color get_ray_color(const Ray &ray, const SphereArr &spheres, size_t depth);


const size_t MAX_RAY_COLOR_RECURSION_DEPTH      = 4;
const size_t ANTI_ALIASING_PROCESSING_PER_PIXEL = 25;
const size_t LAMBERT_RAYS_QUANTITY              = 2;


struct IntersectionData
{
public:

    Vector3d intersection_coordinates_{};
    Vector3d normal_{};
    float distance_from_origin_ = 0;
    bool is_outer_normal_ = true;


    void set_normal_correct_orientation(const Ray &ray, const Vector3d &outer_normal)
    {
        is_outer_normal_ = ray.direction_ * outer_normal < 0;
        normal_ = is_outer_normal_ ? outer_normal : -outer_normal;
    }
};


#endif
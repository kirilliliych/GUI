#ifndef MATERIAL_HPP
#define MATERIAL_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include "vector3d.hpp"
#include "color.hpp"


struct Material
{
    Vector3d albedo_{};
    float refraction_coef_ = 0;

    Material(){};

    Material(const Vector3d &albedo, float refraction_coef)
      :  albedo_(albedo),
         refraction_coef_(refraction_coef)
    {
        assert(std::isfinite(refraction_coef));
    }
};


#endif
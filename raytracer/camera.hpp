#ifndef CAMERA_HPP
#define CAMERA_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include "ray.hpp"
#include "../utilities/utilities.hpp"


class Camera
{
public:
//-------------------------------------------------------------------------------------
    Camera();

    Camera(const Vector3d &origin, const Vector3d &lookat, const Vector3d &vector_up, 
           float fov, float ratio);
//-------------------------------------------------------------------------------------
    void create(const Vector3d &origin, const Vector3d &lookat, const Vector3d &vector_up,
                float fov, float ratio);

    Ray get_ray(float x_viewport_pos, float y_viewport_pos) const;

//-------------------------------------------------------------------------------------    
    Vector3d origin_{0, 0, 0};
    Vector3d horizontal_component_{0, 0, 0};
    Vector3d vertical_component_{0, 0, 0};
    Vector3d lower_left_corner_{0, 0, 0};
};


#endif
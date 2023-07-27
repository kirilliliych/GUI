#ifndef SCENE_HPP
#define SCENE_HPP


#include "camera.hpp"
#include "material.hpp"
#include "materiallibrary.hpp"
#include "sphere.hpp"


struct Scene
{
//---------------------------------------------------
    Scene()
      : camera_(Camera())
    {}

    Scene(const Vector3d &camera_origin, const Vector3d &camera_lookat, const Vector3d &camera_vector_up,
          float fov, float window_sizes_ratio)
      : camera_(Camera(camera_origin, camera_lookat, camera_vector_up,
                       fov, window_sizes_ratio))
    {}

    ~Scene()
    {
        for (size_t sphere_index = 0; sphere_index < spheres_.size_; ++sphere_index)
        {
            delete spheres_.array_[sphere_index];
        }
    }
//---------------------------------------------------
    void add(Sphere *new_sphere)
    {
        spheres_.add(new_sphere);
    }

    void fill_scene(const unsigned raytracer_width, const unsigned raytracer_height);
//---------------------------------------------------
    Camera camera_;                                         
    SphereArr spheres_{};
};


#endif
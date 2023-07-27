#ifndef SPHERE_HPP
#define SPHERE_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
// #include <SFML/Window.hpp>
// #include <SFML/Graphics.hpp>
#include "../geometry/point3d.hpp"
#include "../geometry/vector3d.hpp"
#include "material.hpp"


const size_t MAX_SPHERES_QUANTITY_SCENE = 1024;


class ImageSf;


class Sphere
{
private:

    Vector3d origin_{};
    Material material_{};
    Color    color_{};
    
    float radius_ = 0;

    bool is_light_source_ = false;

public:

    Sphere(){};

    Sphere(const Point3d &origin, float radius, const Material &material, const Color &color, bool is_light_source)
      : radius_          (radius),
        material_        (material),
        color_           (color),
        is_light_source_ (is_light_source)
    {
        assert(std::isfinite(radius));
        assert(radius > 0);

        origin_.set_x(origin.get_x());
        origin_.set_y(origin.get_y());
        origin_.set_z(origin.get_z());
    }
    
    Sphere(const Vector3d &origin, float radius, const Material &material, const Color &color, bool is_light_source)
      : origin_          (origin),
        radius_          (radius),
        material_        (material),
        color_           (color),
        is_light_source_ (is_light_source)
    {
        assert(std::isfinite(radius));
        assert(radius > 0);
    }
    
    Sphere(float x_origin, float y_origin, float z_origin, float radius, const Material &material, const Color &color, bool is_light_source)
      :  radius_          (radius),
         material_        (material),
         color_           (color),
         is_light_source_ (is_light_source)
    {
        assert(std::isfinite(radius));
        assert(radius > 0);

        origin_.set_x(x_origin);
        origin_.set_y(y_origin);
        origin_.set_z(z_origin);
    }

    float get_x_origin() const
    {
        return origin_.get_x();
    }
    float get_y_origin() const
    {
        return origin_.get_y();
    }
    float get_z_origin() const 
    {
        return origin_.get_z();
    }
    Vector3d get_origin() const
    {
        return origin_;
    }
    const Vector3d &get_origin_ref() const
    {
        return origin_;
    }

    float get_radius() const
    {
        return radius_;
    }
    Material get_material() const
    {
        return material_;
    }
    Color get_color() const
    {
        return color_;
    }

    bool is_light_source()
    {
        return is_light_source_;
    }

    void set_x_origin(float x_origin)
    {
        origin_.set_x(x_origin);
    }
    void set_y_origin(float y_origin)
    {
        origin_.set_y(y_origin);
    }
    void set_z_origin(float z_origin)
    {
        origin_.set_z(z_origin);
    }
    void set_radius(float radius)
    {
        assert(std::isfinite(radius));
        assert(radius > 0);

        radius_ = radius;
    }

    void set_material(Material material)
    {
        material_ = material;
    }

    void set_color(Color color)
    {
        color_ = color;
    }
};


struct SphereArr
{
//---------------------------------------------------
    SphereArr()
      : size_(0)
    {}

    ~SphereArr(){}
//---------------------------------------------------

    int add(Sphere *new_sphere)
    {
        assert(new_sphere != nullptr);

        if (size_ >= MAX_SPHERES_QUANTITY_SCENE)
        {
            std::cerr << "sphere was not added to array due to overflow\n";
            
            return 1;
        }

        array_[size_] = new_sphere;
        ++size_;

        return 0;
    }

    Sphere *array_[MAX_SPHERES_QUANTITY_SCENE] = {};
    size_t size_ = 0;
};


#endif

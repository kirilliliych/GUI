#include "ray.hpp"


bool ray_intersects_sphere(const Sphere &sphere, 
                           const Ray &ray, 
                           float dist_min, 
                           float dist_max, 
                           IntersectionData *data)
{
    assert(data != nullptr);

    Vector3d from_sphere_center_to_ray_origin = ray.origin_ - sphere.get_origin_ref();

    float a_coef = ray.direction_ * ray.direction_;
    float b_coef = from_sphere_center_to_ray_origin * ray.direction_ * 2.0;
    float c_coef = from_sphere_center_to_ray_origin *
                   from_sphere_center_to_ray_origin -
                   get_square(sphere.get_radius());
    
    float discriminant = get_square(b_coef) - 4 * a_coef * c_coef;
    if (discriminant < 0)
    {
        return false;
    }
    float discr_sqrt = sqrt(discriminant);

    float potential_distance = (-b_coef - discr_sqrt) / (2.0 * a_coef);
    if ((potential_distance < dist_min) || (dist_max < potential_distance))
    {
          potential_distance = (-b_coef + discr_sqrt) / (2.0 * a_coef);

        if ((potential_distance < dist_min) || (dist_max < potential_distance))
        {
            return false;
        }
    }

    data->intersection_coordinates_ = ray.get_position_at(potential_distance);
    data->distance_from_origin_ = potential_distance;
    Vector3d outer_normal = data->intersection_coordinates_ - sphere.get_origin_ref();
    normalize_vector(&outer_normal);
    data->set_normal_correct_orientation(ray, outer_normal);

    return true;
}

bool get_closest_intersection(const Ray &ray, const SphereArr &spheres, IntersectionData *data, Sphere **closest_sphere)
{
    assert(data            != nullptr);
    assert(closest_sphere  != nullptr);
    assert(*closest_sphere != nullptr);

    bool is_something_intersected = false;
    float closest_intersection_distance = INFINITY;

    for (size_t sphere_index = 0; sphere_index < spheres.size_; ++sphere_index)
    {
        IntersectionData cur_sphere_data{};

        Sphere *cur_sphere = spheres.array_[sphere_index];
        if (ray_intersects_sphere(*cur_sphere, ray, EPSILON_DISTANCE, closest_intersection_distance, &cur_sphere_data))
        {
            is_something_intersected = true;
            closest_intersection_distance = cur_sphere_data.distance_from_origin_;
            *data = cur_sphere_data;
            *closest_sphere = cur_sphere;
        }
    }

    return is_something_intersected;
}

Color get_background_color(const Vector3d &normalized_ray_direction)
{
    float grad_parameter = 0.5 * (normalized_ray_direction.get_y() + 1.0);

    return Color{WHITE} * (1.0 - grad_parameter) + Color{LIGHT_BLUE} * grad_parameter;
}

Color get_Lambert_ray_color(const IntersectionData &data, const Sphere *closest_sphere, const SphereArr &spheres, size_t depth)
{
    assert(closest_sphere != nullptr);

    const Vector3d &sphere_albedo = closest_sphere->get_material().albedo_;

    Vector3d congregated_color{0, 0, 0};
    if (sphere_albedo.get_x() > FLOAT_COMPARISON_PRECISION)
    {
        for (size_t Lambert_ray_index = 0; Lambert_ray_index < LAMBERT_RAYS_QUANTITY; ++Lambert_ray_index)              
        {
            Vector3d new_direction = data.normal_ + get_random_unit_vector_normal_halfsphere(data.normal_);
            if (is_close_to_zero(new_direction))
            {
                new_direction = data.normal_;
            }
            congregated_color += color_to_vector(get_ray_color(Ray{data.intersection_coordinates_, new_direction}, spheres, depth - 1) * 
                                                 closest_sphere->get_color() *
                                                 closest_sphere->get_material().albedo_.get_x());
        }
    }

    return vector_to_color(congregated_color / LAMBERT_RAYS_QUANTITY);
}

Color get_reflected_color(const Ray &ray, const IntersectionData &data, Sphere *closest_sphere, const SphereArr &spheres, size_t depth)
{
    assert(closest_sphere != nullptr);

    const Vector3d &sphere_albedo = closest_sphere->get_material().albedo_;

    if (sphere_albedo.get_y() > FLOAT_COMPARISON_PRECISION)
    {
        Vector3d reflected = get_reflected(ray.direction_, data.normal_);
        return  get_ray_color(Ray{data.intersection_coordinates_, reflected}, spheres, depth - 1) * 
                closest_sphere->get_material().albedo_.get_y() * 
                closest_sphere->get_color();
    }

    return Color{(float) 0, (float) 0, (float) 0};
}

Color get_refracted_color(const Ray &ray, const IntersectionData &data, Sphere *closest_sphere, const SphereArr &spheres, size_t depth)
{
    assert(closest_sphere != nullptr);

    const Vector3d &sphere_albedo = closest_sphere->get_material().albedo_;

    if (sphere_albedo.get_z() > FLOAT_COMPARISON_PRECISION)
    {
        float refraction_index = data.is_outer_normal_ ? (1.0 / closest_sphere->get_material().refraction_coef_) :      
                                                                closest_sphere->get_material().refraction_coef_; 

        bool if_refraction = true;
        Vector3d refracted = get_refracted(ray.direction_, data.normal_, refraction_index, &if_refraction);

        Color temp_color = get_ray_color(Ray{data.intersection_coordinates_, refracted}, spheres, depth - 1) *
                        closest_sphere->get_material().albedo_.get_z();  
  
        return (if_refraction) ?
               temp_color :
               temp_color * closest_sphere->get_color();
    }

    return Color{(float) 0, (float) 0, (float) 0};
}


Color get_ray_color(const Ray &ray, const SphereArr &spheres, size_t depth)
{   
    if (depth == 0)
    {
        return Color{BLACK};
    }

    IntersectionData data{};
    Sphere *closest_sphere = spheres.array_[0];
    if (get_closest_intersection(ray, spheres, &data, &closest_sphere))                                                     // find closest object
    {
        if (closest_sphere->is_light_source())
        {
            return closest_sphere->get_color();
        }

        Color ray_color{BLACK};
        const Vector3d &sphere_albedo = closest_sphere->get_material().albedo_;

        ray_color += get_Lambert_ray_color(   data, closest_sphere, spheres, depth);
        ray_color += get_reflected_color(ray, data, closest_sphere, spheres, depth);
        ray_color += get_refracted_color(ray, data, closest_sphere, spheres, depth);

        return ray_color;
    }

    Vector3d normalized_ray_direction = ray.direction_;
    normalize_vector(&normalized_ray_direction);

    return get_background_color(normalized_ray_direction);
}

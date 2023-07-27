#include "camera.hpp"


Camera::Camera()
  : origin_              (Vector3d{0, 0, 0}),
    horizontal_component_(Vector3d{0, 0, 0}),
    vertical_component_  (Vector3d{0, 0, 0}),
    lower_left_corner_   (Vector3d{0, 0, 0})
{}

Camera::Camera(const Vector3d &origin, const Vector3d &lookat, const Vector3d &vector_up, float fov, float ratio)
{
    create(origin, lookat, vector_up, fov, ratio);
}


void Camera::create(const Vector3d &origin, const Vector3d &lookat, const Vector3d &vector_up,
                    float fov, float ratio)
{
    assert(std::isfinite(fov));
    assert(std::isfinite(ratio));

    float height = tan(degrees_to_radians(fov) / 2);
    float viewport_height = height * 2.0;
    float viewport_width  = viewport_height * ratio;

    Vector3d z_axis = origin - lookat;
    normalize_vector(&z_axis);
    Vector3d x_axis = get_perpendicular(z_axis, vector_up);
    normalize_vector(&x_axis);
    Vector3d y_axis = get_perpendicular(x_axis, z_axis);
    normalize_vector(&y_axis);

    origin_ = origin;
    horizontal_component_ = -x_axis * viewport_width;
    vertical_component_   =  y_axis * viewport_height;

    lower_left_corner_ = origin_ - horizontal_component_ / 2 - vertical_component_ / 2 - z_axis;
}


Ray Camera::get_ray(float x_viewport_pos, float y_viewport_pos) const
{
    assert(std::isfinite(x_viewport_pos));
    assert(std::isfinite(y_viewport_pos));

    return Ray{origin_, lower_left_corner_ + horizontal_component_ * x_viewport_pos
                                           + vertical_component_   * y_viewport_pos
                                           - origin_};
}
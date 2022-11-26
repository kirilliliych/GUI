#include "vector3d.hpp"


Vector3d::Vector3d(float x_coord, float y_coord, float z_coord)
  : x_coord_(x_coord),
    y_coord_(y_coord),
    z_coord_(z_coord)
{
    assert(std::isfinite(x_coord));
    assert(std::isfinite(y_coord));
    assert(std::isfinite(z_coord));
}

Vector3d::Vector3d(const Point3d *beginning, const Point3d *ending)
{
    assert(beginning != nullptr);
    assert(ending    != nullptr);

    x_coord_ = ending->get_x() - beginning->get_x();
    y_coord_ = ending->get_y() - beginning->get_y();
    z_coord_ = ending->get_z() - beginning->get_z();
}

Vector3d::Vector3d(const Point3d *radius_vector_ending)
{
    assert(radius_vector_ending != nullptr);

    x_coord_ = radius_vector_ending->get_x();
    y_coord_ = radius_vector_ending->get_y();
    z_coord_ = radius_vector_ending->get_z(); 
}

void normalize_vector(Vector3d *vector)
{
    assert(vector != nullptr);

    float denominator = *vector * *vector;
    assert(denominator != 0);

    double sqrt_denom = sqrt(denominator);
    vector->set_x(vector->get_x() / sqrt_denom);
    vector->set_y(vector->get_y() / sqrt_denom);
    vector->set_z(vector->get_z() / sqrt_denom);
}

void set_vector_length(Vector3d *vector, float length)
{
    assert(vector != nullptr);

    normalize_vector(vector);
    *vector *= length;
}

float cos_between_vectors(const Vector3d &opd1, const Vector3d &opd2)
{
    return (opd1 * opd2) / (sqrtf(opd1 * opd1) * 
                            sqrtf(opd2 * opd2));
}

Vector3d get_perpendicular(const Vector3d &v1, const Vector3d &v2)
{
    return Vector3d{v1.get_y() * v2.get_z() - v1.get_z() * v2.get_y(),
                    v1.get_z() * v2.get_x() - v1.get_x() * v2.get_z(),
                    v1.get_x() * v2.get_y() - v1.get_y() * v2.get_x()
                   };
}

Vector3d get_reflected(const Vector3d &cur_vector, const Vector3d &normal_vector)
{
    return cur_vector - normal_vector * 2 * cos_between_vectors(cur_vector, normal_vector);
}

Vector3d get_refracted(const Vector3d &cur_vector, const Vector3d &normal_vector, 
                       float refraction_index,     bool *if_refraction)
{
    assert(std::isfinite(refraction_index));
    assert(if_refraction != nullptr);

    float cos_in = cos_between_vectors(-cur_vector, normal_vector);
    float sin_in = sqrtf(1.0 - cos_in * cos_in);

    if (refraction_index * sin_in > 1.0)
    {
        *if_refraction = false;

        return get_reflected(cur_vector, normal_vector);
    }    

    Vector3d reflected_perpendicular_to_normal_component = (cur_vector + normal_vector * cos_in) * refraction_index;
    Vector3d reflected_parallel_to_normal_component      = normal_vector * -sqrtf(fabs(1.0 - reflected_perpendicular_to_normal_component *
                                                                                             reflected_perpendicular_to_normal_component));
    *if_refraction = true;

    return reflected_perpendicular_to_normal_component + reflected_parallel_to_normal_component;
}

Vector3d get_random_vector()
{
    return Vector3d{get_random_fractional_float(), get_random_fractional_float(), get_random_fractional_float()};
}

Vector3d get_random_vector(float min, float max)
{
    return Vector3d{get_random_fractional_float(min, max), get_random_fractional_float(min, max), get_random_fractional_float(min, max)};
}

Vector3d get_random_unit_vector()
{
    Vector3d generated_vector{};
    do
    {
        generated_vector = get_random_vector(-1, 1);
    }
    while ((generated_vector * generated_vector >= 1) || 
           (generated_vector * generated_vector < FLOAT_COMPARISON_PRECISION));
    
    normalize_vector(&generated_vector);

    return generated_vector;
}

Vector3d get_random_unit_vector_normal_halfsphere(const Vector3d &normal)
{
    Vector3d generated_vector = get_random_unit_vector();

    return generated_vector * normal > 0 ? generated_vector : -generated_vector;
}

bool is_close_to_zero(const Vector3d &vector)
{
    return (fabs(vector.get_x()) < FLOAT_COMPARISON_PRECISION) &&
           (fabs(vector.get_y()) < FLOAT_COMPARISON_PRECISION) &&
           (fabs(vector.get_z()) < FLOAT_COMPARISON_PRECISION);
}
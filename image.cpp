#include "image.hpp"


Image::Image(unsigned width, unsigned height, Color color)
{
    create(width, height, color);
    //std::cout << "image constructor called" << std::endl;
}

void Image::create(unsigned width, unsigned height, Color color)
{
    width_  = width;
    height_ = height;

    image_.create(width, height, color.get_color_ref());

    //std::cout << "image was created" << std::endl;
}

void Image::set_pixel(unsigned x_coord, unsigned y_coord, Color color)
{
    image_.setPixel(x_coord, y_coord, color.get_color_ref());
}

// void Image::render(const Camera &camera, const SphereArr &spheres)
// {
//     for (int y_pos = 0; y_pos < height_; ++y_pos)
//     {
//         for (int x_pos = 0; x_pos < width_; ++x_pos)
//         {
//             Vector3d congregated_color{0, 0, 0};
//             for (int pixel_processing_num = 0; pixel_processing_num < ANTI_ALIASING_PROCESSING_PER_PIXEL; ++pixel_processing_num)        // LEGACY CODE
//             {
//                 float x_viewport_pos = (get_random_fractional_float() + x_pos) / (width_  - 1);
//                 float y_viewport_pos = (get_random_fractional_float() + y_pos) / (height_ - 1);

//                 congregated_color += color_to_vector(get_ray_color(camera.get_ray(x_viewport_pos, y_viewport_pos), 
//                                                      spheres, 
//                                                      MAX_RAY_COLOR_RECURSION_DEPTH));
//             }

//             Color result_color = vector_to_color(congregated_color / ANTI_ALIASING_PROCESSING_PER_PIXEL);

//             set_pixel(x_pos, y_pos, result_color.gamma_correction(GAMMA_CORRECTION_COEF));
//         }
//     }
// }

void Image::render_pixel(const Camera &camera, const SphereArr &spheres, unsigned x, unsigned y)
{
    Vector3d congregated_color{0, 0, 0};
    for (int pixel_processing_num = 0; pixel_processing_num < ANTI_ALIASING_PROCESSING_PER_PIXEL; ++pixel_processing_num)
    {
        float x_viewport_pos = (get_random_fractional_float() + x) / (width_  - 1);
        float y_viewport_pos = (get_random_fractional_float() + y) / (height_ - 1);

        congregated_color += color_to_vector(get_ray_color(camera.get_ray(x_viewport_pos, y_viewport_pos), 
                                             spheres, 
                                             MAX_RAY_COLOR_RECURSION_DEPTH));
    }

    Color result_color = vector_to_color(congregated_color / ANTI_ALIASING_PROCESSING_PER_PIXEL);

    set_pixel(x, (height_ - 1) - y, result_color.gamma_correction(GAMMA_CORRECTION_COEF));
}

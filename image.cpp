#include "image.hpp"


Image::Image()
{}

Image::Image(int width, int height, const Color &color)
{
    create(width, height, color);
}

Image::Image(const char *file_name)
{
    assert(file_name != nullptr);

    load_from_file(file_name);

    sf::Vector2u size = image_.getSize();
    width_  = size.x;
    height_ = size.y;
}


void Image::create(int width, int height, const Color &color)
{
    width_  = width;
    height_ = height;

    image_.create(width, height, color.get_color_ref());
}

bool Image::is_created() const
{
    return image_.getSize() != sf::Vector2u{0, 0};
}

bool Image::load_from_file(const char *file_name)
{
    assert(file_name != nullptr);

    return image_.loadFromFile(file_name);
}

Color Image::get_pixel(int x_coord, int y_coord)
{
    sf::Color result = image_.getPixel(static_cast<unsigned> (x_coord), 
                                       static_cast<unsigned> (y_coord));

    return Color{result.toInteger()};
}

void Image::set_pixel(int x_coord, int y_coord, Color color)
{
    image_.setPixel(x_coord, y_coord, color.get_color_ref());
}

int Image::get_width() const
{
    return width_;
}

int Image::get_height() const
{
    return height_;
}


void Image::render_pixel(const Camera &camera, const SphereArr &spheres, int x, int y)
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

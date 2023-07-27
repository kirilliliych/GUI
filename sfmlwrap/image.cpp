#include "../sfmlwrap/image.hpp"


ImageSf::ImageSf()
{}

ImageSf::ImageSf(int width, int height, const Color &color)
{
    create(width, height, color);
}

ImageSf::ImageSf(const char *file_name)
{
    assert(file_name != nullptr);

    load_from_file(file_name);

    sf::Vector2u size = image_.getSize();
    width_  = size.x;
    height_ = size.y;
}


void ImageSf::create(int width, int height, const Color &color)
{
    width_  = width;
    height_ = height;

    image_.create(width, height, color.get_color_ref());
}

bool ImageSf::is_created() const
{
    return image_.getSize() != sf::Vector2u{0, 0};
}

bool ImageSf::load_from_file(const char *file_name)
{
    assert(file_name != nullptr);

    return image_.loadFromFile(file_name);
}

void ImageSf::copy(const ImageSf &source, int dest_x, int dest_y)
{
    image_.copy(source.image_, static_cast<unsigned> (dest_x), static_cast<unsigned> (dest_y));
}

Color ImageSf::get_pixel(int x_coord, int y_coord)
{
    sf::Color result = image_.getPixel(static_cast<unsigned> (x_coord), 
                                       static_cast<unsigned> (y_coord));

    return Color{result.toInteger()};
}

void ImageSf::set_pixel(int x_coord, int y_coord, Color color)
{
    image_.setPixel(x_coord, y_coord, sf::Color{color.get_uint32_color()});
}

int ImageSf::get_width() const
{
    return width_;
}

int ImageSf::get_height() const
{
    return height_;
}


void ImageSf::render_pixel(const Camera &camera, const SphereArr &spheres, int x, int y)
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

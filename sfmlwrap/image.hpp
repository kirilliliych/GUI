#ifndef IMAGE_HPP
#define IMAGE_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <cfloat>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../raytracer/camera.hpp"
#include "../utilities/color.hpp"
#include "../geometry/point2d.hpp"
#include "../raytracer/ray.hpp"
#include "../raytracer/sphere.hpp"
#include "texture.hpp"


class  Sphere;
struct SphereArr;
class  Camera;
class  Window;
class Surface;
class Texture;



class ImageSf
{
    friend Surface;
    friend Texture;

public:
//----------------------------------------------------------------------------------
    ImageSf();

    ImageSf(int width, int height, const Color &color = WHITE);

    ImageSf(const char *file_name);

    ~ImageSf() = default;
//----------------------------------------------------------------------------------
    void create(int width, int height, const Color &color = WHITE);

    bool is_created() const;

    bool load_from_file(const char *file_name);

    void copy(const ImageSf &source, int dest_x, int dest_y);

    Color get_pixel(int x_coord, int y_coord);
    void set_pixel(int x_coord, int y_coord, Color color);

    int get_width()  const;
    int get_height() const;

    void render_pixel(const Camera &camera, const SphereArr &spheres, int x, int y);

private:
public:
    int width_  = 0;
    int height_ = 0;

    sf::Image image_{};
};

#endif
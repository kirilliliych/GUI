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
#include "color.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "ray.hpp"


class  Sphere;
struct SphereArr;
class  Camera;
class  Window;
class Surface;

class Image
{
    friend Surface;

private:

    unsigned width_  = 0;
    unsigned height_ = 0;

    sf::Image image_{};

public:

    Image(){};

    Image(unsigned width, unsigned height, Color color = Color{WHITE});

    void create(unsigned width, unsigned height, Color color = Color{WHITE});

    void set_pixel(unsigned x_coord, unsigned y_coord, Color color);

    void render(const Camera &camera, const SphereArr &spheres);

    void render_pixel(const Camera &camera, const SphereArr &spheres, unsigned x, unsigned y);

    unsigned get_width() const
    {
        return width_;
    }
    unsigned get_height() const
    {
        return height_;
    }


};

#endif
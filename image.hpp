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
#include "camera.hpp"
#include "color.hpp"
#include "point2d.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "texture.hpp"


class  Sphere;
struct SphereArr;
class  Camera;
class  Window;
class Surface;
class Texture;


class Image
{
    friend Surface;
    friend Texture;

public:
//----------------------------------------------------------------------------------
    Image();

    Image(int width, int height, const Color &color = WHITE);

    Image(const char *file_name);

   ~Image() = default;
//----------------------------------------------------------------------------------
    void create(int width, int height, const Color &color = WHITE);

    bool is_created() const;

    bool load_from_file(const char *file_name);

    Color get_pixel(int x_coord, int y_coord);
    void set_pixel(int x_coord, int y_coord, Color color);


    int get_width()  const;
    int get_height() const;


    void render_pixel(const Camera &camera, const SphereArr &spheres, int x, int y);

private:

    int width_  = 0;
    int height_ = 0;

    sf::Image image_{};


};

#endif
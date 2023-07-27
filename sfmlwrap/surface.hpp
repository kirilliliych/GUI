#ifndef SURFACE_HPP
#define SURFACE_HPP


#include <cassert>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../utilities/color.hpp"
#include "image.hpp"
#include "../geometry/point2d.hpp"
#include "rectangle.hpp"
#include "texture.hpp"
#include "sprite.hpp"


class Sprite;
class Text;


class Surface
{
public:
//---------------------------------------------------------

    Surface(){}

    Surface(int width, int height);

    ~Surface(){}
//---------------------------------------------------------

    bool create(int width, int height);

    void clear(const Color &color = Color{0x000000FF});

    void update();

    void draw_line(const Point2d &point1, const Point2d &point2, const Color &color);

    void draw_rectangle(const Rectangle &rectangle, const Color &color);

    void draw_rectangle(const Rectangle &rectangle, const Point2d &point, const Color &color);

    void draw_image(const ImageSf &image);

    void draw_sprite(const Sprite &sprite);

    void draw_sprite(const Point2d &start, const Sprite &sprite);

    void draw_text(const Text &text);

    void draw_point(const Point2d &point, const Color &color);

    Point2d get_size() const;

    const Texture &get_texture() const;
    

private:
//public:

    void update_texture_();

//---------------------Variables---------------------------
    sf::RenderTexture surface_{};
    Texture           surface_texture_{};
//---------------------------------------------------------
};


#endif
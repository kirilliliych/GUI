#include "surface.hpp"
#include "text.hpp"


Surface::Surface(int width, int height)
{
    assert(surface_.create(width, height));
    update();
}

bool Surface::create(int width, int height)
{
    return surface_.create(width, height);
}

void Surface::clear(const Color &color)
{
    surface_.clear(sf::Color(color.get_uint32_color()));
    
}

void Surface::update()
{
    surface_.display();
    update_texture_();
}

void Surface::draw_line(const Point2d &point1, const Point2d &point2, const Color &color)
{
    sf::Vertex vertices[2] = {};
    vertices[0].position = sf::Vector2f{point1.x, point1.y};
    vertices[1].position = sf::Vector2f{point2.x, point2.y};

    vertices[0].color = vertices[1].color = sf::Color{color.get_uint32_color()};

    surface_.draw(vertices, 2, sf::Lines);
    
    update();
}


void Surface::draw_rectangle(const Rectangle &rectangle, const Color &color)
{
    draw_rectangle(rectangle, {0, 0}, color);
}

void Surface::draw_rectangle(const Rectangle &rectangle, const Point2d &start, const Color &color)
{
    sf::RectangleShape temp_rect(sf::Vector2f{static_cast<float> (rectangle.get_width()),
                                                static_cast<float> (rectangle.get_height())
                                                });
        temp_rect.setPosition(start.x, start.y);
        temp_rect.setFillColor(sf::Color(color.get_uint32_color()));

        surface_.draw(temp_rect);
        
        update();
}

void Surface::draw_image(const ImageSf &image)
{
    sf::RectangleShape picture(sf::Vector2f{static_cast<float> (image.get_width()), 
                                            static_cast<float> (image.get_height())
                                           });
    sf::Texture picture_texture{};
    picture_texture.loadFromImage(image.image_);
    picture.setTexture(&picture_texture);

    surface_.draw(picture);

    update();
}


void Surface::draw_sprite(const Sprite &sprite)
{
    surface_.draw(sprite.sprite_);

    update();
}

void Surface::draw_sprite(const Point2d &start, const Sprite &sprite)
{
    Sprite temp_sprite = sprite;
    temp_sprite.set_position(start);
    surface_.draw(temp_sprite.sprite_);

    update();
}

void Surface::draw_text(const Text &text)
{
    surface_.draw(text.text_);
    
    update();
}

void Surface::draw_point(const Point2d &point, const Color &color)
{
    sf::Vertex vertex{sf::Vector2f{point.x, point.y}, sf::Color{color.get_uint32_color()}};
    surface_.draw(&vertex, 1, sf::Points);

    update();
}

Point2d Surface::get_size() const
{
    sf::Vector2u size = surface_.getSize();

    return Point2d{static_cast<int> (size.x), static_cast<int> (size.y)};
}


const Texture &Surface::get_texture() const
{
    return surface_texture_;
}


void Surface::update_texture_()
{
    surface_texture_.texture_ = surface_.getTexture();
}

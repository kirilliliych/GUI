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

void Surface::clear(Color color)
{
    surface_.clear(sf::Color(color.get_uint32_color()));
}

void Surface::update()
{
    surface_.display();
    update_texture_();
}

void Surface::draw_line(const Point2d &point1, const Point2d &point2, const Color color)
{
    sf::Vertex vertices[2] = {};
    vertices[0].position = sf::Vector2f{point1.get_x(), point1.get_y()};
    vertices[1].position = sf::Vector2f{point2.get_x(), point2.get_y()};

    vertices[0].color = vertices[1].color = sf::Color{color.get_uint32_color()};

    surface_.draw(vertices, 2, sf::Lines);
    
    update();
}


void Surface::draw_rectangle(const Rectangle &rectangle, const Color color)
{
    sf::RectangleShape temp_rect(sf::Vector2f{static_cast<float> (rectangle.get_width()),
                                              static_cast<float> (rectangle.get_height())
                                             });
    temp_rect.setPosition(0, 0);
    temp_rect.setFillColor(sf::Color(color.get_uint32_color()));

    surface_.draw(temp_rect);
    
    update();
}

//void Surface::draw_frame(const Rec)

void Surface::draw_image(const Image &image)
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

// void Surface::draw_surface(const Point2d &at, const Surface &surface, const Rectangle &rectangle)               // not tested
// {
//     sf::Sprite temp_sprite(surface.get_texture().texture_);
//     temp_sprite.setPosition(sf::Vector2f{at.get_x(), at.get_y()});
//     if ((rectangle.get_x()      == 0) &&
//         (rectangle.get_y()      == 0) &&
//         (rectangle.get_width()  == 0) &&
//         (rectangle.get_height() == 0))
//     {
//         temp_sprite.setTextureRect(sf::IntRect{static_cast<int> (rectangle.get_x()),
//                                                static_cast<int> (rectangle.get_y()),
//                                                static_cast<int> (rectangle.get_width()),
//                                                static_cast<int> (rectangle.get_height())
//                                               });
//     }

//     surface_.draw(temp_sprite);

//     update();
// }

void Surface::draw_sprite(const Sprite &sprite)
{
    surface_.draw(sprite.sprite_);

    update();
}

void Surface::draw_text(const Text &text)
{
    // std::cout << "drawing text to surface" << std::endl;
    // Color text_color = text.get_color();
    // text_color.make_transparent();
    // printf("text background color: %x\n", text_color.get_uint32_color());
    // draw_rectangle(text.get_area(), text_color);

    surface_.draw(text.text_);

    // sf::Image img = surface_.getTexture().copyToImage();
    // img.createMaskFromColor(sf::Color{0xFFFF00FF});
    // sf::Texture txt{};
    // txt.loadFromImage(img);
    // sf::Sprite spr{};
    // spr.setTexture(txt);
    // surface_.draw(spr);
    
    update();
    
}

const Texture &Surface::get_texture() const
{
    return surface_texture_;
}



void Surface::update_texture_()
{
    surface_texture_.texture_ = surface_.getTexture();
}

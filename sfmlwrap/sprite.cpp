#include "sprite.hpp"


Sprite::Sprite(const Texture &texture)
  : sprite_{sf::Sprite(texture.texture_)}
{}

Sprite::Sprite(const Texture &texture, const Rectangle &rectangle)
  : sprite_{sf::Sprite(texture.texture_, to_SFML_IntRect(rectangle))}
{}

Sprite &Sprite::operator =(const Sprite &other)
{
    *this = other;

    return *this;
}

void Sprite::set_texture(const Texture &texture)
{
    sprite_.setTexture(texture.texture_);
}

void Sprite::set_texture_rect(const Rectangle &rectangle)
{
    sprite_.setTextureRect(to_SFML_IntRect(rectangle));
}

void Sprite::set_position(Point2d position)
{
    set_position(position.x, position.y);
}
void Sprite::set_position(float x, float y)
{
    assert(std::isfinite(x));
    assert(std::isfinite(y));

    sprite_.setPosition(sf::Vector2f{x, y});
}

void Sprite::set_origin(Point2d origin)
{
    set_origin(origin.x, origin.y);
}
void Sprite::set_origin(float x, float y)
{
    assert(std::isfinite(x));
    assert(std::isfinite(y));

    sprite_.setOrigin(sf::Vector2f{x, y});
}

void Sprite::load_from_surface(const Surface *surface)
{
    set_texture(surface->get_texture());
}


#ifndef SPRITE_HPP
#define SPRITE_HPP


#include "surface.hpp"
#include "texture.hpp"


class Surface;
class Window;

class Sprite
{
    friend Surface;
    friend Window;

public:
//----------------------------------------------------------------------------------------------------
    Sprite(){}

    Sprite(const Texture &texture);

    Sprite(const Texture &texture, const Rectangle &rectangle);

    ~Sprite(){}

     Sprite &operator =(const Sprite &other);   // ?
//----------------------------------------------------------------------------------------------------

    void set_texture(const Texture &texture);

    void set_texture_rect(const Rectangle &rectangle);

    void set_position(Point2d position);
    void set_position(float x, float y);

    void set_origin(Point2d origin);
    void set_origin(float x, float y);
    
    void load_from_surface(const Surface *surface);

private:

    sf::Sprite sprite_;
};


#endif

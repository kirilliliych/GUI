#ifndef TEXTURE_HPP
#define TEXTURE_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class Surface;
class Sprite;

class Texture
{
    friend Surface;
    friend Sprite;

public:
//-------------Ctor, dtor...-------------------    
    Texture(){}

    Texture(const char *file_name)
    {
        assert(file_name != nullptr);

        load_from_file(file_name);
    }

    ~Texture(){}
//-------------Functions-----------------------

bool load_from_file(const char *file_name)
{
    return texture_.loadFromFile(file_name);
}

void set_repeated(bool is_repeated)
{
    texture_.setRepeated(is_repeated);
}


Texture &operator =(const Texture &other_texture)
{
    *this = other_texture;

    return *this;
}

private:
    
    sf::Texture texture_{};
};


#endif
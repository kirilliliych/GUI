#ifndef TEXTURE_HPP
#define TEXTURE_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../sfmlwrap/image.hpp"


class Surface;
class Sprite;
class ImageSf;


class Texture
{
    friend Surface;
    friend Sprite;

public:
//-----------------------------------------------------
    Texture();

    Texture(const char *file_name);

    ~Texture();
//-----------------------------------------------------
    bool load_from_file(const char *file_name);

    bool load_from_image(const ImageSf &image);

    ImageSf copy_to_image() const;

    void set_repeated(bool is_repeated);

    Texture &operator =(const Texture &other_texture);
//---------------------Variables-----------------------
private:
//public:
    sf::Texture texture_{};
};


#endif
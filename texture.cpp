#include "texture.hpp"


Texture::Texture()
{}

Texture::Texture(const char *file_name)
{
    assert(file_name != nullptr);

    load_from_file(file_name);
}

Texture::~Texture()
{}

bool Texture::load_from_file(const char *file_name)
{
    return texture_.loadFromFile(file_name);
}

bool Texture::load_from_image(const Image &image)
{
    return texture_.loadFromImage(image.image_);
}

void Texture::set_repeated(bool is_repeated)
{
    texture_.setRepeated(is_repeated);
}


Texture &Texture::operator =(const Texture &other_texture)
{
    *this = other_texture;

    return *this;
}

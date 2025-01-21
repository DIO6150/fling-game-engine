
#include "stb_image/stb_image.h"

#include "texture.hpp"

FGE::Texture::Texture (std::string path)
{
    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(path.c_str(), &width, &height, &nb_channels, 4);

    if (!data)
    {
        printf ("%s(%s) : (%s) %s\n", __func__, __FILE__, path.c_str (), stbi_failure_reason ());
    }
}

FGE::Texture::~Texture ()
{
    stbi_image_free(data);
}

FGE::Texture::Texture (const Texture& other)
{
    width = other.width;
    height = other.height;
    nb_channels = other.nb_channels;
    data = other.data;
}

FGE::Texture::Texture (Texture&& other)
{
    width = other.width;
    height = other.height;
    nb_channels = other.nb_channels;
    data = other.data;

    other.data = nullptr;
}

FGE::Texture& FGE::Texture::operator=(const Texture& other)
{
    width = other.width;
    height = other.height;
    nb_channels = other.nb_channels;
    data = other.data;

    return (*this);
}
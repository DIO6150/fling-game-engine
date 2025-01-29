#pragma once

#include <string>

namespace FGE
{
    class Texture
    {
    public:
        int width, height, nb_channels;
        unsigned char* data;

        Texture (std::string path);
        ~Texture ();

        Texture (const Texture& other);
        Texture (Texture&& other);

        Texture& operator=(const Texture& other);

    };
}
#pragma once

#include <unordered_map>
#include <string>

namespace FGE
{
    class TextureAtlas;

    struct TextureQuad
    {
        TextureAtlas* atlas;
        float x, y, w, h;
    };
    
    class TextureAtlas
    {
        unsigned int m_object;

        std::unordered_map<std::string, TextureQuad> m_atlas;

        TextureQuad m_missing_texture;

        int m_width = 0;
        int m_height = 0;

        int m_cumul_width  = 0;
        int m_cumul_height = 0;
        int m_cumul_depth  = 0;

        int m_max_height = 0;

        int m_max_size = 4096;
        int m_max_depth = 1;

    public:
        TextureAtlas (unsigned int n);
        TextureAtlas () : TextureAtlas (0) {}
        ~TextureAtlas ();

        TextureQuad* GetQuad (std::string texture_key);
        int GetMaxSize () { return (m_max_size); }
        unsigned int GetObject () { return (m_object); }
    };
}
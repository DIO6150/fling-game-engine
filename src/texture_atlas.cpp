
#include "texture_atlas.hpp"

#include "engine.hpp"

#include "texture.hpp"

FGE::TextureAtlas::TextureAtlas (unsigned int n)
{
    glActiveTexture(GL_TEXTURE0 + n);
    glGenTextures(1, &m_object);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_object);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, m_max_size, m_max_size, m_max_depth);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    // Missing Texture

    m_missing_texture = (TextureQuad) { this, 0, 0, 2, 2 };

    unsigned char data[16] = {
        207,   8, 174, 255,      0,   0,   0, 255,
          0,   0,   0, 255,    207,   8, 174, 255
    };

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
        0,
        0, 0, 0,
        2, 2, 1,
        GL_RGBA, GL_UNSIGNED_BYTE,
        data
    );

    m_cumul_width = 2;
    m_cumul_height = 2;
    m_max_height = 2;
}

FGE::TextureAtlas::~TextureAtlas ()
{

}

FGE::TextureQuad* FGE::TextureAtlas::GetQuad (std::string texture_key)
{
    if (texture_key == "")
    {
        return (&m_missing_texture);
    }

    std::unordered_map<std::string, TextureQuad>::iterator found = m_atlas.find (texture_key);

    if (found != m_atlas.end()) return (&found->second);

    Texture* texture = FGE::Engine::FindGameObject<Texture> (texture_key);
    if (!texture) return (nullptr);

    if (m_max_height < texture->height)
    {
        m_max_height = texture->height;
    }

    if (m_cumul_width + texture->width > m_max_size)
    {
        m_cumul_height += texture->height;
        m_cumul_width = 0;
        m_max_height = 0;
    }

    if (m_cumul_height + texture->height > m_max_size)
    {
        printf ("Too many textures in atlas.\n");

        return (nullptr);
    }

    m_atlas [texture_key] = (TextureQuad) { this, (float)m_cumul_width, (float)m_cumul_height, (float)texture->width, (float)texture->height };

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
        0,
        m_cumul_width, m_cumul_height, 0,
        texture->width, texture->height, 1,
        GL_RGBA, GL_UNSIGNED_BYTE,
        texture->data
    );

    m_cumul_width += texture->width;

    return (&m_atlas [texture_key]);
}
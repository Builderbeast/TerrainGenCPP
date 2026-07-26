#pragma once

#include <vector>

class Texture
{
private:
    unsigned int m_RendererID;
    int m_Width, m_Height, m_BPP;
public:
    Texture(const std::vector<unsigned char>& pixelData, int width, int height);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};
#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <string>

#include <glad/glad.h>

namespace rose {


class Texture {
    public:
        Texture();
        virtual ~Texture() {}
        void LoadTexture(const std::string& path);
        GLuint GetRendererID() const { return m_RendererID; }
        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
    private:
        int32_t m_Width;
        int32_t m_Height;
        GLuint m_RendererID;
};


}


#endif //SPRITE_SHEET_H

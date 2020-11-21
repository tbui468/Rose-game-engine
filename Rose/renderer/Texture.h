#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>


namespace rose {

enum class TextureType {
    None,
    Default,
    Custom
};

struct Sprite {
    glm::ivec2 TexCoords;
    glm::ivec2 TexDimensions;
    TextureType TexType {TextureType::None};
};


class Texture {
    public:
        Texture(int texSlot);
        virtual ~Texture() {}
        void LoadTexture(const std::string& path);
//        GLuint GetRendererID() const { return m_RendererID; }
        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
        int GetSlot() const { return m_TextureSlot; }
        void Bind();
    private:
        int32_t m_Width;
        int32_t m_Height;
        GLuint m_RendererID;
        int m_TextureSlot;
};


}


#endif //SPRITE_SHEET_H

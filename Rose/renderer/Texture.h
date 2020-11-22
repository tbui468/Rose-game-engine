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
        Texture(int width, int height);
        Texture(const std::string& path);
        virtual ~Texture() {}
        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
        void Bind();
        void CopySubTexture(const glm::ivec2& startDest, GLuint srcID, const glm::ivec2& startSrc, int width, int height);
        GLuint GetID() const { return m_RendererID; }
    private:
        void LoadTexture(const std::string& path);
    private:
        int32_t m_Width {0};
        int32_t m_Height {0};
        GLuint m_RendererID;
};


}


#endif //SPRITE_SHEET_H

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "renderer/Texture.h"


namespace rose {

Texture::Texture(const std::string& path) {
    glGenTextures(1, &m_RendererID);
    Bind(); 
    LoadTexture(path);
}

Texture::Texture(int width, int height) : m_Width(width), m_Height(height) {
    glGenTextures(1, &m_RendererID);
    Bind(); 

    const int channels = 4;
    uint8_t* buffer = new uint8_t[width * height * channels];

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    delete buffer;
}

//only binding to slot 0 for now
void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}


void Texture::LoadTexture(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);
    int32_t channels;
    uint8_t *data = stbi_load(path.c_str(), &m_Width, &m_Height, &channels, 0);

//    if(stbi_failure_reason()) std::cout << stbi_failure_reason() << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(data);
}


void Texture::CopySubTexture(const glm::ivec2& startDest, GLuint srcID, const glm::ivec2& startSrc, int width, int height) {
    glCopyImageSubData(srcID, GL_TEXTURE_2D, 0, startSrc.x, startSrc.y, 0, 
                       GetID(), GL_TEXTURE_2D, 0, startDest.x, startDest.y, 0,
                       width, height, 1);
}



}

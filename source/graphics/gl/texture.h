#pragma once
#include <glad/glad.h>
#include <stb_image.h>
#include <stdexcept>
#include <string>

namespace gl
{
    class Textures
    {
        GLuint textures = 0;
        GLsizei totalTextures = 0;

    public:

        struct Descriptor
        {
            GLenum format = GL_RGB;
            bool generateMipmap = true;
            GLint wrapS = GL_REPEAT;
            GLint wrapT = GL_REPEAT;
            GLint minFilter = GL_NEAREST_MIPMAP_LINEAR;
            GLint magFilter = GL_NEAREST_MIPMAP_LINEAR;
        };

        Textures(GLsizei totalTextures = 1) : totalTextures(totalTextures) {}

        ~Textures()
        {
            glDeleteTextures(totalTextures, &textures);
        }

        void load(const std::string & filePath, Descriptor && descriptor)
        {
            glGenTextures(totalTextures, &textures);
            glBindTexture(GL_TEXTURE_2D, textures);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, descriptor.wrapS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, descriptor.wrapT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, descriptor.minFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, descriptor.magFilter);

            stbi_set_flip_vertically_on_load(true);
            int width = 0;
            int height = 0;
            int channels = 0;
            stbi_uc* textureData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
            if ( !textureData )
                throw std::logic_error("Failed to load texture!");

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, descriptor.format, GL_UNSIGNED_BYTE, textureData);
            stbi_image_free(textureData);

            if ( descriptor.generateMipmap )
                glGenerateMipmap(GL_TEXTURE_2D);
        }

        void bind(GLenum texture = GL_TEXTURE0)
        {
            glActiveTexture(texture);
            glBindTexture(GL_TEXTURE_2D, textures);
        }
    };
}
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace gl
{
    class Shader
    {
        GLuint shaderId = 0;

        std::basic_string<GLchar> getInfoLog()
        {
            GLint logLength = 0;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
            if ( logLength > 0 )
            {
                auto logText = std::basic_string<GLchar>(logLength, static_cast<GLchar>('\0'));
                glGetShaderInfoLog(shaderId, logLength, NULL, (GLchar*)logText.data());
                if ( logLength > 0 )
                    return logText;
            }
            return std::basic_string<GLchar>{};
        }

    public:

        enum class Type : GLenum
        {
            fragment = GL_FRAGMENT_SHADER,
            vertex = GL_VERTEX_SHADER
        };

        Shader() = delete;
        Shader(Shader &) = delete;
        Shader & operator=(const Shader &) = delete;

        Shader(Shader::Type type, const char* source, size_t sourceLength, bool requireNonEmpty)
        {
            if ( requireNonEmpty && (source == nullptr || source[0] == '\0') )
                throw std::runtime_error("The shader source cannot be empty!");

            GLint length = static_cast<GLint>(sourceLength);
            shaderId = glCreateShader(static_cast<GLenum>(type));
            glShaderSource(shaderId, 1, (const GLchar* const*)&source, &length);
            glCompileShader(shaderId);

            GLint status = 0;
            glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
            if ( status == GL_FALSE )
                throw std::runtime_error("Failed to compile shader: \n\n" + getInfoLog());
        }

        Shader(Shader::Type type, std::string_view source, bool requireNonEmpty = true)
            : Shader(type, source.data(), source.size(), requireNonEmpty) {}

        Shader(Shader && other) noexcept : shaderId(other.shaderId) {
            other.shaderId = 0;
        }

        Shader & operator=(Shader && other) noexcept {
            shaderId = other.shaderId;
            other.shaderId = 0;
        }

        ~Shader()
        {
            if ( shaderId != 0 )
                glDeleteShader(shaderId);
        }

        GLuint getShaderId() { return shaderId; }
    };

    template <Shader::Type ShaderType>
    Shader shaderFromFile(const std::string & filePath, bool requireNonEmpty = true)
    {
        std::ifstream shaderFile {};
        shaderFile.exceptions(std::ios_base::badbit | std::ios_base::failbit);
        shaderFile.open(filePath);
        if ( !shaderFile )
            throw std::runtime_error("FNF");
        std::vector<char> buffer{std::istreambuf_iterator<char>(shaderFile), std::istreambuf_iterator<char>()};
        return Shader(ShaderType, buffer.data(), buffer.size(), requireNonEmpty);
    }

    Shader vertexShaderFromFile(const std::string & filePath, bool requireNonEmpty = true)
    {
        return shaderFromFile<Shader::Type::vertex>(filePath, requireNonEmpty);
    }

    Shader fragmentShaderFromFile(const std::string & filePath, bool requireNonEmpty = true)
    {
        return shaderFromFile<Shader::Type::fragment>(filePath, requireNonEmpty);
    }
}

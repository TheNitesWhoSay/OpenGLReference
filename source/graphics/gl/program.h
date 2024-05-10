#pragma once
#include "shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <unordered_map>

namespace gl
{
    class Program
    {
        GLuint programId = 0;
        std::vector<Shader> shaders {};
        std::unordered_map<std::string, GLint> uniformLocationCache {};

    public:

        ~Program()
        {
            if ( this->programId != 0 )
                glDeleteProgram(this->programId);
        }

        void create()
        {
            this->programId = glCreateProgram();
        }

        void attachShader(Shader && shader)
        {
            glAttachShader(this->programId, shader.getShaderId());
            shaders.push_back(std::move(shader));
        }

        void link()
        {
            glLinkProgram(this->programId);
        }

        void use()
        {
            glUseProgram(this->programId);
        }

        template <typename T>
        void setUniform(const std::string & name, T && value)
        {
            static constexpr auto validateIsCurrentProgram = [](GLint expectedProgramId) {
                GLint currentProgram = 0;
                glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
                if ( currentProgram == 0 )
                    throw std::logic_error("Program::setUniform(...) was called prior to setting a program");
                else if ( currentProgram != expectedProgramId )
                {
                    throw std::logic_error("Program::setUniform(...) was called for program " + std::to_string(expectedProgramId)
                        + " but program " + std::to_string(currentProgram) + " was currently set!");
                }
            };

            auto getUniformLocation = [&]() {
                #ifdef _DEBUG
                validateIsCurrentProgram(this->programId); // Perform this check every time in debug mode
                #endif
                
                if ( auto found = uniformLocationCache.find(name); found != uniformLocationCache.end() )
                    return found->second;

                #ifndef _DEBUG
                validateIsCurrentProgram(this->programId); // Perform this check on first access only in release mode
                #endif

                // First time trying to access this uniform, need to find the uniform location
                if ( auto location = glGetUniformLocation(this->programId, name.c_str()); location != -1 )
                {
                    uniformLocationCache.emplace(name, location);
                    return location;
                }

                // Failed to get uniform location, diagnose the issue
                {
                    if ( this->programId == 0 )
                        throw std::logic_error("Program::setUniform(...) called prior to program initialization!");
                    else if ( shaders.empty() )
                        throw std::logic_error("Program::setUniform(...) called without attaching any shaders!");

                    GLint linkStatus = 0;
                    glGetProgramiv(this->programId, GL_LINK_STATUS, &linkStatus);
                    if ( linkStatus == GL_FALSE )
                        throw std::logic_error("Program::setUniform(...) called prior to program linking!");

                    for ( auto & shader : shaders )
                    {
                        if ( shader.getShaderId() == 0 )
                        {
                            throw std::logic_error("Program::setUniform(...) failed for uniform name \"" + name
                                + "\", this is most likely due to an uninitialized shader");
                        }

                        GLint compileStatus = 0;
                        glGetShaderiv(shader.getShaderId(), GL_COMPILE_STATUS, &compileStatus);
                        if ( compileStatus == GL_FALSE )
                        {
                            throw std::logic_error("Program::setUniform(...) failed to uniform name \"" + name
                                + "\", this is most likely due to a shader compilation error in shader "
                                + std::to_string(shader.getShaderId()));
                        }
                    }

                    throw std::invalid_argument("Uniform name: \"" + name
                        + "\" could not be set, the uniform name is most likely invalid!");
                }
            };

            using type = std::_Remove_cvref_t<T>;
            auto location = getUniformLocation();

            if constexpr ( std::is_same_v<type, bool> )
                glUniform1i(location, static_cast<GLint>(value));
            else if constexpr ( std::is_same_v<type, GLfloat> )
                glUniform1f(location, value);
            else if constexpr ( std::is_same_v<type, GLint> )
                glUniform1i(location, value);
            else if constexpr ( std::is_same_v<type, GLuint> )
                glUniform1ui(location, value);
            else if constexpr ( std::is_same_v<type, glm::mat4> )
                glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
            else
                static_assert(std::is_void_v<type>, "Expected a bool or GL float, int, or uint");
        }
    };
}
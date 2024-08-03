#pragma once
#include "graphics/gl/program.h"
#include "graphics/gl/texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace refapp
{
    class Containers
    {
        struct Shader : gl::Program
        {
            gl::uniform::Mat4 view {"view"};
            gl::uniform::Mat4 model {"model"};
            gl::uniform::Mat4 projection {"projection"};
            gl::uniform::Float mixPolarity {"mixPolarity"};
            gl::uniform::Int texture0 {"texture0"};
            gl::uniform::Int texture1 {"texture1"};

            void load() {
                gl::Program::create();
                gl::Program::attachShader(gl::shaderFromFile<gl::Shader::Type::vertex>("res/shader/vertex.glsl"));
                gl::Program::attachShader(gl::shaderFromFile<gl::Shader::Type::fragment>("res/shader/fragment.glsl"));
                gl::Program::link();
                gl::Program::use();
                gl::Program::findUniforms(view, model, projection, mixPolarity, texture0, texture1);
                view.loadIdentity();
                model.loadIdentity();
                projection.loadIdentity();
                mixPolarity.setValue(0.2f);
            }
        };

        gl::Texture texture0 {};
        gl::Texture texture1 {};
        GLuint vao = 0;
        GLuint vbo = 0;

        static constexpr GLfloat verticies[] {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
        };

        static inline glm::vec3 locations[] {
            glm::vec3( 0.0f, 0.0f, 0.0f),
            glm::vec3( 2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f, 2.0f, -2.5f),
            glm::vec3( 1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
        };

        void bind()
        {
            glBindVertexArray(vao);
            texture0.bindToSlot(GL_TEXTURE0);
            texture1.bindToSlot(GL_TEXTURE1);
        }

    public:

        Shader shader {};
        float mixPolarity = 0.2f;

        void load()
        {
            shader.load();
            texture0.genTexture();
            texture0.bind();
            texture0.loadImage2D("res/texture/container.jpg", gl::Texture::Image2D{.internalformat = GL_RGB, .format = GL_RGB});
            texture0.setMinMagFilters(GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            texture1.genTexture();
            texture1.bind();
            texture1.loadImage2D("res/texture/awesomeface.png", gl::Texture::Image2D{.internalformat = GL_RGB, .format = GL_RGBA});
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
            glEnableVertexAttribArray(0); // Position
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(GLfloat)));
            glEnableVertexAttribArray(2); // Texture

            shader.use();
            shader.texture0.setValue(0);
            shader.texture1.setValue(1);
            shader.mixPolarity.setValue(mixPolarity);
        }

        void draw()
        {
            bind();

            for ( size_t i=0; i<10; ++i )
            {
                auto & location = locations[i];
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, location);
                GLfloat angle = 20.0f * i;
                if ( i % 3 == 0 )
                    model = glm::rotate(model, float(glfwGetTime())*glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
                else
                    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                shader.model.setMat4(glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    };
}

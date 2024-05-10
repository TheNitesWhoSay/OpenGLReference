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
        gl::Textures texture0 {};
        gl::Textures texture1 {};
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
            texture0.bind(GL_TEXTURE0);
            texture1.bind(GL_TEXTURE1);
            glBindVertexArray(vao);
        }

    public:

        float mixPolarity = 0.2f;

        void load(gl::Program & shaders)
        {
            texture0.load("res/texture/container.jpg", {
                .format = GL_RGB,
                .minFilter = GL_NEAREST,
                .magFilter = GL_NEAREST
            });

            texture1.load("res/texture/awesomeface.png", {
                .format = GL_RGBA
            });

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
            glEnableVertexAttribArray(0); // Position
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(GLfloat)));
            glEnableVertexAttribArray(2); // Texture

            shaders.use();
            shaders.setUniform("texture0", 0);
            shaders.setUniform("texture1", 1);
            shaders.setUniform("mixPolarity", mixPolarity);
        }

        void draw(gl::Program & shaders)
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

                shaders.setUniform("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    };
}

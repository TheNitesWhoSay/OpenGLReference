#pragma once
#include <gl/program.h>
#include <gl/texture.h>
#include <gl/utils.h>
#include <gl/vertices.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
                gl::Program::attachShader(gl::shaderFromFile<gl::Shader::Type::vertex>("res/shader/container-vertex.glsl"));
                gl::Program::attachShader(gl::shaderFromFile<gl::Shader::Type::fragment>("res/shader/container-fragment.glsl"));
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
            shader.use();
            texture0.bindToSlot(GL_TEXTURE0);
            texture1.bindToSlot(GL_TEXTURE1);
        }

    public:

        Shader shader {};
        float mixPolarity = 0.2f;
        gl::VertexVector<> verts;

        void load()
        {
            shader.load();
            texture0.genTexture();
            texture0.bind();
            texture0.loadImage2D("res/texture/container.jpg", gl::Texture::Image2D{.internalformat = GL_RGB, .format = GL_RGB});
            texture0.setMinMagFilters(GL_NEAREST);
            texture0.setTextureWrap(GL_REPEAT);
            
            texture1.genTexture();
            texture1.bind();
            texture1.loadImage2D("res/texture/awesomeface.png", gl::Texture::Image2D{.internalformat = GL_RGB, .format = GL_RGBA});
            texture1.generateMipmap();
            texture1.setTextureWrap(GL_REPEAT);

            gl::Texture::bindDefault();

            shader.use();
            shader.texture0.setValue(0);
            shader.texture1.setValue(1);
            shader.mixPolarity.setValue(mixPolarity);

            verts.initialize({
                gl::VertexAttribute{.size = 3}, // Position.xyz
                gl::VertexAttribute{.size = 2}  // TexCoord.xy
            });
            verts.vertices.insert(verts.vertices.begin(), std::begin(this->verticies), std::end(this->verticies));
            verts.bind();
            verts.bufferData(gl::UsageHint::StaticDraw);
            gl::unbind();
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
                verts.bind();
                verts.drawTriangles();
            }
        }
    };
}

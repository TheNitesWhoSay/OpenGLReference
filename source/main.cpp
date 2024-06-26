﻿#include "graphics/glfw/context.h"
#include "graphics/glfw/window.h"
#include "graphics/glad/utils.h"
#include "graphics/gl/camera.h"
#include "graphics/gl/program.h"
#include "graphics/gl/utils.h"
#include "containers.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace refapp
{
    struct App
    {
        gl::Camera camera {};
        gl::Program shaders {};
        glfw::Window window {};
        refapp::Containers containers {};
        double deltaTime = 0.0f;
        double lastFrame = 0.0f;
        double lastX = 400;
        double lastY = 300;

        void updateDelta()
        {
            auto currentFrame = glfwGetTime();
            this->deltaTime = currentFrame - this->lastFrame;
            this->lastFrame = currentFrame;
        }

        void clearWindow()
        {
            gl::clearColor(0.2f, 0.3f, 0.3f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void cursorPosChanged(double xPos, double yPos)
        {
            static bool firstMouse = true;

            if ( firstMouse )
            {
                lastX = xPos;
                lastY = yPos;
                firstMouse = false;
            }

            double xOffset = xPos - lastX;
            double yOffset = lastY - yPos;
            lastX = xPos;
            lastY = yPos;

            constexpr float sensitivity = 0.1f;
            xOffset *= sensitivity;
            yOffset *= sensitivity;
            camera.yaw += float(xOffset);
            camera.pitch += float(yOffset);

            if ( camera.pitch > 89.0f )
                camera.pitch = 89.0f;
            else if ( camera.pitch < -89.0f)
                camera.pitch = -89.0f;

            glm::vec3 direction {};
            direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
            direction.y = sin(glm::radians(camera.pitch));
            direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
            camera.front = glm::normalize(direction);
        }

        void wheelScrolled(double xOffset, double yOffset)
        {
            camera.zoom -= (float)yOffset;
            if ( camera.zoom < 1.0f )
                camera.zoom = 1.0f;
            else if ( camera.zoom > 45.0f )
                camera.zoom = 45.0f;
        }

        void processInput()
        {
            if ( window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS )
                window.setWindowShouldClose(true);
            else if ( window.getKey(GLFW_KEY_UP) == GLFW_PRESS )
            {
                containers.mixPolarity += 0.01f;
                shaders.setUniform("mixPolarity", containers.mixPolarity);
            }
            else if ( window.getKey(GLFW_KEY_DOWN) == GLFW_PRESS )
            {
                containers.mixPolarity -= 0.01f;
                shaders.setUniform("mixPolarity", containers.mixPolarity);
            }

            const float cameraSpeed = 2.5f * float(deltaTime);
            if ( window.getKey(GLFW_KEY_W) == GLFW_PRESS )
                camera.pos += cameraSpeed * camera.front;
            if ( window.getKey(GLFW_KEY_S) == GLFW_PRESS )
                camera.pos -= cameraSpeed * camera.front;
            if ( window.getKey(GLFW_KEY_A) == GLFW_PRESS )
                camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
            if ( window.getKey(GLFW_KEY_D) == GLFW_PRESS )
                camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
        }

        void createMainWindow()
        {
            constexpr int initialWidth = 800;
            constexpr int initialHeight = 600;
            window.setVersionHint(3, 3);
            window.setCoreProfileHint();
            window.create(initialWidth, initialHeight, "LearnOpenGL");
            window.enableForwarding(this);

            window.makeContextCurrent();
            glad::loadGL();

            gl::enableDepthTesting();
            gl::setViewport(0, 0, initialWidth, initialHeight);

            window.setFrameBufferSizeCallback<[](GLFWwindow* window, int width, int height) { gl::setViewport(0, 0, width, height); }>();
            window.setCursorPosCallback<&App::cursorPosChanged>();
            window.setScrollCallback<&App::wheelScrolled>();

            window.setCursorDisabledInputMode();
        }

        void loadShaders()
        {
            shaders.create();
            shaders.attachShader(gl::vertexShaderFromFile("res/shader/vertex.glsl"));
            shaders.attachShader(gl::fragmentShaderFromFile("res/shader/fragment.glsl"));
            shaders.link();
        }

        void run()
        {
            glfw::Context glfwContext {};
            createMainWindow();
            loadShaders();
            containers.load(shaders);
            
            while ( !window.shouldClose() )
            {
                updateDelta();
                processInput();
                clearWindow();
                camera.update(shaders);

                containers.draw(shaders);

                gl::unbind();
                window.pollEvents();
                window.swapBuffers();
            }

        }
    };
}

int main()
{
    refapp::App{}.run();
    return 0;
}

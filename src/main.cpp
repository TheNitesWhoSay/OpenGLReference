#include <glfw/context.h>
#include <glfw/window.h>
#include <glad/utils.h>
#include <gl/camera.h>
#include <gl/font.h>
#include <gl/fps.h>
#include <gl/program.h>
#include <gl/utils.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "containers.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>

namespace refapp
{
    struct App
    {
        gl::Camera camera {};
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
                containers.shader.mixPolarity.setValue(containers.mixPolarity);
            }
            else if ( window.getKey(GLFW_KEY_DOWN) == GLFW_PRESS )
            {
                containers.mixPolarity -= 0.01f;
                containers.shader.mixPolarity.setValue(containers.mixPolarity);
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
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            gl::enableDepthTesting();
            gl::setViewport(0, 0, initialWidth, initialHeight);

            window.setFrameBufferSizeCallback<[](GLFWwindow* window, int width, int height) { gl::setViewport(0, 0, width, height); }>();
            window.setCursorPosCallback<&App::cursorPosChanged>();
            window.setScrollCallback<&App::wheelScrolled>();

            window.setCursorDisabledInputMode();
        }

        void run()
        {
            glfw::Context glfwContext {};
            createMainWindow();
            containers.load();

            auto teal16Pt = gl::Font::load("res/fonts/Lato.ttf", 0, 16*64, 144, 144);
            teal16Pt->setColor(0.f, 1.f, 1.f);
            auto black16pt = gl::Font::load("res/fonts/Lato.ttf", 0, 16*64, 144, 144);
            black16pt->setColor(0.f, 0.f, 0.f);

            gl::Fps fps {};
            while ( !window.shouldClose() )
            {
                fps.update(std::chrono::system_clock::now());
                containers.shader.use();
                updateDelta();
                processInput();
                clearWindow();

                camera.update(containers.shader);
                containers.draw();

                teal16Pt->drawAffixedText<gl::Align::Right>(790.0f, 10.0f, fps.displayNumber, " fps", "");
                black16pt->drawText<gl::Align::Center>(400.f, 550.f, "The quick brown fox jumped over the lazy dog.");

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

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "program.h"

namespace gl
{
    struct Camera
    {
        static inline const glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

        float yaw = -90.0f;
        float pitch = 0.0f;

        float zoom = 45.0f;

        glm::vec3 pos =  glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 direction = glm::normalize(pos - target);
        glm::vec3 right = glm::normalize(glm::cross(upDirection, direction));
        glm::vec3 up = glm::cross(direction, right);

        void update(gl::Program & shaders)
        {
            glm::mat4 view = glm::lookAt(pos, pos + front, up);
            glm::mat4 projection = glm::perspective(glm::radians(zoom), 800.0f/600.0f, 0.1f, 100.0f);

            shaders.setUniform("view", view);
            shaders.setUniform("projection", projection);
        }

    };
}

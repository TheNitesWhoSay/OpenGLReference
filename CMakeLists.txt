cmake_minimum_required (VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

project("OpenGLReference")

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG ae721c5 # 2024-02-13
)
FetchContent_MakeAvailable(stb)
include_directories(${stb_SOURCE_DIR}/include)

add_executable(OpenGLReference "source/main.cpp"  "source/graphics/glfw/context.h" "source/graphics/glfw/window.h" "source/graphics/glad/utils.h" "source/graphics/gl/utils.h" "source/graphics/gl/camera.h" "source/graphics/gl/shader.h" "source/graphics/gl/texture.h" "source/containers.h" "source/graphics/stb/stb.cpp")
target_link_libraries(OpenGLReference glad::glad glfw glm::glm)

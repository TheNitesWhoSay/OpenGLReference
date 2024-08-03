cmake_minimum_required (VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

project("OpenGLReference")

find_package(glad CONFIG REQUIRED)
get_target_property(GLAD_INCLUDE_DIRS glad::glad INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${GLAD_INCLUDE_DIRS})

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

include(FetchContent)
FetchContent_Declare(
  rarecpp
  GIT_REPOSITORY https://github.com/TheNitesWhoSay/RareCpp.git
  GIT_TAG d437771 # release-2.3.2
)
FetchContent_MakeAvailable(rarecpp)
include_directories(${rarecpp_SOURCE_DIR}/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/graphics/)

set(Headers
  "source/graphics/glfw/context.h"
  "source/graphics/glfw/window.h"
  "source/graphics/glad/utils.h"
  "source/graphics/gl/base.h"
  "source/graphics/gl/camera.h"
  "source/graphics/gl/context_semaphore.h"
  "source/graphics/gl/palette.h"
  "source/graphics/gl/program.h"
  "source/graphics/gl/shader.h"
  "source/graphics/gl/texture.h"
  "source/graphics/gl/uniform.h"
  "source/graphics/gl/unique_resource.h"
  "source/graphics/gl/utils.h"
  "source/graphics/gl/vertices.h"
)
set(Sources
  "source/main.cpp"
  "source/graphics/stb/stb.cpp"
)

add_executable(OpenGLReference ${Sources} ${Headers})
target_link_libraries(OpenGLReference glad::glad glfw glm::glm)

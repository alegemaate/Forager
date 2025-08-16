cmake_minimum_required(VERSION 3.22)

project(Forager VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target)


# Find libs
include(cmake/get_cpm.cmake)
CPMAddPackage("gh:adsgames/asw@0.5.10")
CPMAddPackage("gh:nlohmann/json@3.11.3")
CPMAddPackage("gh:g-truc/glm#1.0.1")


find_library(ALLEGRO_LIBRARY NAMES liballeg44.dll.a liballeg.dll.a liballeg.so liballeg.dylib)
find_library(ALLEGRO_PNG_LIBRARY NAMES libloadpng.dll.a libloadpng.so libloadpng.dylib)
find_library(ALLEGRO_GL_LIBRARY NAMES liballeggl.dll.a liballeggl.so liballeggl.dylib)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)

# Source code
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
include_directories(${GLEW_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

# Link Libs
target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${ALLEGRO_LIBRARY}
  ${ALLEGRO_PNG_LIBRARY}
  ${ALLEGRO_GL_LIBRARY}
  asw
  nlohmann_json::nlohmann_json
  OpenGL::GL
  GLEW::GLEW
  glm::glm
  -lGLU
)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

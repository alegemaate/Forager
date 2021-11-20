cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

project(Forager)

# Find libs
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
if (MINGW)
    target_link_libraries(${PROJECT_NAME} -lmingw32)
endif (MINGW)

target_link_libraries(${PROJECT_NAME} ${ALLEGRO_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_PNG_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_GL_LIBRARY})
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++ -lm)
cmake_minimum_required(VERSION 3.22)

project(Forager VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target)

# Find libs
include(cmake/get_cpm.cmake)
CPMAddPackage("gh:adsgames/asw#v0.13.0")
CPMAddPackage("gh:nlohmann/json@3.11.3")
CPMAddPackage("gh:g-truc/glm#1.0.1")

# Source code
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

# Link Libs
target_link_libraries(${PROJECT_NAME} PRIVATE 
  asw
  nlohmann_json::nlohmann_json
  glm::glm
)

# Web page with asw's shell, loading progress and preloaded assets
asw_add_web_target(${PROJECT_NAME}
  TITLE "Forager"
  ASSETS ${CMAKE_CURRENT_LIST_DIR}/assets
)

if(EMSCRIPTEN)
  target_link_options(${PROJECT_NAME} PRIVATE
    -sWASM=1
    -sMAXIMUM_MEMORY=1gb
    -sSTACK_SIZE=32mb
    -sINITIAL_MEMORY=64mb
    -sFULL_ES3
  )
else()
  find_package(OpenGL REQUIRED COMPONENTS OpenGL)
  find_package(GLEW REQUIRED)
  include_directories(${GLEW_INCLUDE_DIRS})

  target_link_libraries(${PROJECT_NAME} PRIVATE 
    GLEW::GLEW
    OpenGL::GL
  )
endif()

file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

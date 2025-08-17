#ifndef GLOBALS_H
#define GLOBALS_H

#include "../core/Camera.h"
#include "../core/GpuProgram.h"

extern GpuProgram* defaultShader;
extern GpuProgram* waterShader;
extern GpuProgram* skyShader;

extern Camera* camera;

extern float skyTime;

extern glm::vec3 lightDir;
extern glm::vec3 lightColor;

constexpr size_t CHUNK_WIDTH = 16;
constexpr size_t CHUNK_HEIGHT = 128;
constexpr size_t CHUNK_LENGTH = 16;

constexpr size_t WORLD_WIDTH = 8;
constexpr size_t WORLD_LENGTH = 8;
constexpr size_t WORLD_HEIGHT = 1;

#endif  // GLOBALS_H

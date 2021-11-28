#ifndef GLOBALS_H
#define GLOBALS_H

#include "../core/Camera.h"
#include "../core/GpuProgram.h"

extern GpuProgram* defaultShader;
extern GpuProgram* waterShader;
extern GpuProgram* skyShader;

extern Camera* camera;

extern const float PI_F;

extern float skyTime;
extern float sunX;
extern float sunY;
extern float sunZ;

constexpr size_t CHUNK_WIDTH = 16;
constexpr size_t CHUNK_HEIGHT = 16;
constexpr size_t CHUNK_LENGTH = 16;

constexpr size_t WORLD_WIDTH = 4;
constexpr size_t WORLD_LENGTH = 4;

#endif  // GLOBALS_H

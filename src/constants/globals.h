#ifndef GLOBALS_H
#define GLOBALS_H

#include "../core/GpuProgram.h"

extern bool gameMode;

extern GpuProgram* defaultShader;
extern GpuProgram* waterShader;
extern GpuProgram* skyShader;

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

constexpr size_t DEFAULT_MAP_HEIGHT = CHUNK_HEIGHT;
constexpr size_t DEFAULT_MAP_WIDTH = CHUNK_WIDTH * WORLD_WIDTH;
constexpr size_t DEFAULT_MAP_LENGTH = CHUNK_LENGTH * WORLD_LENGTH;

#endif  // GLOBALS_H

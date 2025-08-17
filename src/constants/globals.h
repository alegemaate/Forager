#pragma once

#include "../core/Camera.h"
#include "../core/GpuProgram.h"

extern GpuProgram defaultShader;

extern Camera camera;

extern glm::vec3 lightDir;
extern glm::vec3 lightColor;

constexpr size_t CHUNK_WIDTH = 16;
constexpr size_t CHUNK_HEIGHT = 128;
constexpr size_t CHUNK_LENGTH = 16;

constexpr size_t WORLD_WIDTH = 4;
constexpr size_t WORLD_LENGTH = 4;
constexpr size_t WORLD_HEIGHT = 1;

#include "globals.h"

GpuProgram* defaultShader = nullptr;
GpuProgram* waterShader = nullptr;
GpuProgram* skyShader = nullptr;

Camera* camera = nullptr;

float skyTime = 0.4f;

glm::vec3 lightDir(0.0f, 0.0f, 0.0f);

glm::vec3 lightColor(0.0f, 0.0f, 0.0f);
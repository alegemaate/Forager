#include "globals.h"

GpuProgram* defaultShader = nullptr;
GpuProgram* waterShader = nullptr;
GpuProgram* skyShader = nullptr;

Camera* camera = nullptr;

const float PI_F = 3.14159265358979323846264338327950288f;

float skyTime = 0.4;
float sunX = 0.0f;
float sunY = 0.0f;
float sunZ = 0.0f;

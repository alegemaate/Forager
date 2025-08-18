#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "./GpuProgram.h"
#include "./Logger.h"

class ShaderManager {
 public:
  ShaderManager() = default;
  ~ShaderManager() = default;

  // Load shader from file
  GpuProgram& createShader(const std::string& name) {
    shaders[name] = GpuProgram();
    return shaders[name];
  }

  // Get shader by name
  const GpuProgram& getShader(const std::string& name) const {
    auto it = shaders.find(name);
    if (it == shaders.end()) {
      Logger::warn("ShaderManager::getShader: Shader not found: " + name);
      throw std::runtime_error("Shader not found: " + name);
    }
    return it->second;
  }

 private:
  std::unordered_map<std::string, GpuProgram>
      shaders;  // Map of shader names to shader IDs
};
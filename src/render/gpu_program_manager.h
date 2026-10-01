#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "./gpu_program.h"

class GpuProgramManager
{
  public:
    // Load shader from file
    GpuProgram& createShader(const std::string& name)
    {
        // Construct in place, GpuProgram owns GL handles and cannot be copied
        auto [it, inserted] = shaders.try_emplace(name);
        if (!inserted)
        {
            throw std::runtime_error("Shader already exists: " + name);
        }
        return it->second;
    }

    // Get shader by name
    const GpuProgram& getShader(const std::string& name) const
    {
        auto it = shaders.find(name);
        if (it == shaders.end())
        {
            asw::log::warn("GpuProgramManager::getShader: Shader not found: {}", name);
            throw std::runtime_error("Shader not found: " + name);
        }
        return it->second;
    }

  private:
    // Map of shader names to shader IDs
    std::unordered_map<std::string, GpuProgram> shaders;
};
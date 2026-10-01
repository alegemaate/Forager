/// @file frustum.h
///
/// View frustum for culling axis aligned boxes
///
#pragma once

#include <array>
#include <glm/glm.hpp>

class Frustum
{
  public:
    /// @brief Extract the six planes from a projection * view matrix (Gribb and Hartmann)
    explicit Frustum(const glm::mat4& viewProjection)
    {
        const glm::mat4 m = glm::transpose(viewProjection);

        planes[0] = m[3] + m[0]; // Left
        planes[1] = m[3] - m[0]; // Right
        planes[2] = m[3] + m[1]; // Bottom
        planes[3] = m[3] - m[1]; // Top
        planes[4] = m[3] + m[2]; // Near
        planes[5] = m[3] - m[2]; // Far
    }

    /// @brief Check if a box is at least partly inside the frustum
    bool intersects(const glm::vec3& min, const glm::vec3& max) const
    {
        for (const auto& plane : planes)
        {
            // Corner of the box furthest along the plane normal
            const glm::vec3 positive{
                plane.x >= 0.0f ? max.x : min.x,
                plane.y >= 0.0f ? max.y : min.y,
                plane.z >= 0.0f ? max.z : min.z,
            };

            if (glm::dot(glm::vec3(plane), positive) + plane.w < 0.0f)
            {
                return false;
            }
        }

        return true;
    }

  private:
    std::array<glm::vec4, 6> planes;
};

#include "./world.h"

#include <cmath>
#include <numbers>

#include "../block/block_registry.h"
#include "../game/controls.h"

namespace
{
// Chunks around the spawn that load before play starts
constexpr i32 START_RADIUS = 2;

// Columns searched around the world origin for dry land to spawn on
constexpr i32 SPAWN_SEARCH = 24;

// A day lasts this many seconds
constexpr float DAY_LENGTH = 600.0f;

i32 toBlock(float value)
{
    return static_cast<i32>(std::floor(value + 0.5f));
}
} // namespace

void World::init()
{
    worldShader.initProgramFromFiles({"textured.vert", "textured.frag"});

    // Load sky
    skybox.loadSkybox("assets/images/skybox/front.png", "assets/images/skybox/back.png",
                      "assets/images/skybox/left.png", "assets/images/skybox/right.png", "assets/images/skybox/top.png",
                      "assets/images/skybox/bottom.png");
}

void World::start(u32 seed, const Settings& newSettings)
{
    settings = &newSettings;

    chunks.reset(seed);

    spawned = false;
    time    = 0.4f;
    clock   = 0.0f;

    // Load around the origin until there is somewhere to stand
    player.spawn(glm::vec3(0.0f, static_cast<float>(CHUNK_HEIGHT), 0.0f));
    camera = Camera(player.getEyePosition(), -90.0f, -10.0f);

    updateLight();
}

void World::stop()
{
    chunks.clear();
    spawned = false;
}

float World::getLoadProgress() const
{
    return chunks.getReadiness(player.getPosition(), std::min(START_RADIUS, settings->renderDistance));
}

bool World::isUnderwater() const
{
    const glm::vec3 eye = camera.getPosition();
    return BlockRegistry::get(chunks.getBlock(toBlock(eye.x), toBlock(eye.y), toBlock(eye.z))).isLiquid();
}

void World::trySpawn()
{
    if (chunks.getReadiness(glm::vec3(0.0f), 1) < 1.0f)
    {
        return;
    }

    // Nearest dry land to the origin, or the origin when it is all sea
    glm::ivec2 best(0, 0);
    i32        bestDistance = -1;

    for (i32 z = -SPAWN_SEARCH; z <= SPAWN_SEARCH; z++)
    {
        for (i32 x = -SPAWN_SEARCH; x <= SPAWN_SEARCH; x++)
        {
            const i32 distance = (x * x) + (z * z);
            if (bestDistance >= 0 && distance >= bestDistance)
            {
                continue;
            }

            const i32 surface = chunks.getSurfaceY(x, z);
            if (surface > SEA_LEVEL)
            {
                best         = glm::ivec2(x, z);
                bestDistance = distance;
            }
        }
    }

    const i32 surface = std::max(chunks.getSurfaceY(best.x, best.y), SEA_LEVEL);
    player.spawn(
        glm::vec3(static_cast<float>(best.x), static_cast<float>(surface) + 0.5f + 0.01f, static_cast<float>(best.y)));
    camera.setPosition(player.getEyePosition());
    spawned = true;
}

void World::update(float dt)
{
    clock += dt;

    if (!spawned)
    {
        trySpawn();
        return;
    }

    // Change time
    time += dt / DAY_LENGTH;

    if (asw::input::get_action(controls::TIME_FORWARD))
    {
        time += dt * 0.5f;
    }
    else if (asw::input::get_action(controls::TIME_BACK))
    {
        time -= dt * 0.5f;
    }

    time -= std::floor(time);
    updateLight();

    camera.processLook(*settings, SDL_GetWindowRelativeMouseMode(asw::display::get_window()));
    player.update(*this, dt);
}

void World::updateLight()
{
    const float angle = 2.0F * std::numbers::pi_v<float> * time;

    // Direction the sunlight travels. Down at noon, up at midnight.
    lightDir.y = 100.0f * std::cos(angle);
    lightDir.x = 100.0f * std::cos(angle);
    lightDir.z = -100.0f * std::sin(angle);

    // Height of the sun, 1 at noon, -1 at midnight
    const float sunHeight = -std::cos(angle);
    const float sunUp     = std::clamp((sunHeight * 3.0f) + 0.2f, 0.0f, 1.0f);

    // Warm at sunrise and sunset, white at noon
    constexpr float SUN_STRENGTH = 0.6f;
    lightColor.x                 = (-1.0f * std::pow((2.0F * time) - 1, 2.0f) + 1.15f) * sunUp * SUN_STRENGTH;
    lightColor.y                 = (-0.6f * (std::cos(angle) - 1) + 0.05f) * sunUp * SUN_STRENGTH;
    lightColor.z                 = (-0.6f * (std::cos(angle) - 1) + 0.05f) * sunUp * SUN_STRENGTH;

    const float ambient = -1.0f * std::pow((2.0F * time) - 1, 2.0f) + 1.15f;
    lightAmbient        = glm::vec3(ambient * 0.6f);

    // Sky blue at noon, deep blue at night
    const float daylight = std::clamp(0.5f + (0.5f * sunHeight), 0.0f, 1.0f);
    fogColor             = glm::mix(glm::vec3(0.02f, 0.03f, 0.08f), glm::vec3(0.62f, 0.76f, 0.95f), daylight);
    skyTint              = glm::mix(glm::vec3(0.08f, 0.08f, 0.15f), glm::vec3(1.0f), daylight);
}

void World::stream()
{
    const i32 radius = settings->renderDistance;

    // Load around the origin until the player is placed
    const glm::vec3 center = spawned ? player.getPosition() : glm::vec3(0.0f);
    chunks.stream(center, radius);

    camera.setFieldOfView(static_cast<float>(settings->fieldOfView));
    camera.setFarPlane(static_cast<float>((radius + 2) * CHUNK_WIDTH) * 1.5f);
}

void World::draw()
{
    const glm::mat4 projection = camera.getProjectionMatrix();
    const glm::mat4 view       = camera.getViewMatrix();

    // Under water the fog hides the sky
    const bool underwater = isUnderwater();
    if (underwater)
    {
        const glm::vec3 water = glm::vec3(0.05f, 0.18f, 0.35f) * std::max(lightAmbient.r, 0.3f);
        glClearColor(water.r, water.g, water.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else
    {
        skybox.render(camera, skyTint);
    }

    worldShader.activate();
    worldShader.setMat4("projection", projection);
    worldShader.setMat4("view", view);
    worldShader.setVec3("light.direction", lightDir);
    worldShader.setVec3("light.ambient", lightAmbient);
    worldShader.setVec3("light.color", lightColor);
    worldShader.setVec3("uFogColor", fogColor);
    worldShader.setFloat("uFogFar", static_cast<float>(settings->renderDistance * CHUNK_WIDTH));
    worldShader.setFloat("uTime", clock);
    worldShader.setInt("uUnderwater", underwater ? 1 : 0);
    worldShader.setInt("atlas", 0);

    // Draw map
    chunks.render(worldShader, projection * view, camera.getPosition());
    worldShader.deactivate();
}

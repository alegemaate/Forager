#include "title.h"

#include <GL/glew.h>
#include <cmath>

#include "../block/block_type.h"
#include "../game/controls.h"

namespace
{
// Longest seed that fits in 32 bits
constexpr size_t MAX_SEED_DIGITS = 9;

// Highest random seed. Noise repeats far out, so small seeds give the most variety.
constexpr int MAX_RANDOM_SEED = 99999;
} // namespace

Title::Title(asw::scene::SceneManager<ProgramState>& manager, AppContext& context)
    : Scene(manager), context(context), settingsMenu(context.settings)
{
    std::vector<MenuItem> items = {
        {"Seed",
         [this]
         {
             // Blinking cursor while typing
             const bool cursor = std::fmod(clock, 1.0f) < 0.5f;
             return seedText + (cursor ? "_" : " ");
         },
         nullptr, [this](int) { newSeed(); }},
        {"Play", nullptr, [this] { play(); }, nullptr},
        {"Settings", nullptr, [this] { settingsMenu.open(); }, nullptr},
    };

#ifndef __EMSCRIPTEN__
    // Closing the tab quits on the web
    items.push_back({"Quit", nullptr, [] { asw::core::exit(); }, nullptr});
#endif

    menu.setItems(std::move(items));
    menu.setTop(0.42f);
}

void Title::init()
{
    clock = 0.0f;
    menu.resetFocus();
    newSeed();
    setMouseCaptured(false);
    SDL_StartTextInput(asw::display::get_window());
}

void Title::cleanup()
{
    SDL_StopTextInput(asw::display::get_window());
}

void Title::newSeed()
{
    seedText = std::to_string(asw::random::between(0, MAX_RANDOM_SEED));
}

void Title::editSeed()
{
    for (const char c : asw::input::get_text_input())
    {
        if (c >= '0' && c <= '9' && seedText.size() < MAX_SEED_DIGITS)
        {
            // Typing over a lone zero replaces it
            seedText = seedText == "0" ? std::string(1, c) : seedText + c;
        }
    }

    if (asw::input::get_key_down(asw::input::Key::Backspace) || asw::input::get_key_repeat(asw::input::Key::Backspace))
    {
        if (!seedText.empty())
        {
            seedText.pop_back();
        }
    }
}

void Title::play()
{
    context.seed = seedText.empty() ? 0 : static_cast<u32>(std::stoul(seedText));
    manager.set_next_scene(ProgramState::Game);
}

void Title::update(float dt)
{
    clock += dt;

    if (asw::input::get_action_down(controls::FULLSCREEN))
    {
        context.settings.fullscreen = !context.settings.fullscreen;
        context.settings.apply();
        context.settings.save();
    }

    if (settingsMenu.isOpen())
    {
        settingsMenu.update();
        return;
    }

    editSeed();
    menu.update();
}

void Title::draw()
{
    int width  = 0;
    int height = 0;
    SDL_GetWindowSizeInPixels(asw::display::get_window(), &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto&           gui    = context.gui;
    const glm::vec2 screen = gui.getSize();
    gui.begin();

    // Sky fading to the horizon, with blocks below
    const f32     horizon = screen.y * 0.78f;
    constexpr int BANDS   = 24;
    const f32     band    = horizon / BANDS;
    for (int i = 0; i < BANDS; i++)
    {
        const f32 t = static_cast<f32>(i) / (BANDS - 1);
        gui.rect(0.0f, static_cast<f32>(i) * band, screen.x, band + 1.0f,
                 glm::vec4(glm::mix(glm::vec3(0.25f, 0.45f, 0.82f), glm::vec3(0.7f, 0.84f, 0.97f), t), 1.0f));
    }

    constexpr f32 TILE  = 48.0f;
    const f32     drift = std::fmod(clock * 12.0f, TILE);
    for (f32 x = -drift; x < screen.x; x += TILE)
    {
        gui.blockIcon(BlockID::Grass, x, horizon, TILE);
        for (f32 y = horizon + TILE; y < screen.y; y += TILE)
        {
            gui.blockIcon(BlockID::Dirt, x, y, TILE, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
        }
    }

    // Title
    const f32 bob = std::sin(clock * 2.0f) * 4.0f;
    gui.textShadow("Forager", screen.x / 2.0f, (screen.y * 0.18f) + bob, 12.0f, glm::vec4(1.0f), TextAlign::Center);

    if (settingsMenu.isOpen())
    {
        settingsMenu.draw(gui);
    }
    else
    {
        menu.draw(gui);
        gui.textShadow("Type a seed, or press left or right for a new one", screen.x / 2.0f, screen.y - 40.0f, 2.0f,
                       glm::vec4(1.0f, 1.0f, 1.0f, 0.85f), TextAlign::Center);
    }

    gui.end();

    asw::display::swap_window();
}

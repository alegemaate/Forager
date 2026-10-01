#include "./screenshot.h"

#include <GL/glew.h>
#include <SDL3_image/SDL_image.h>
#include <asw/asw.h>
#include <chrono>
#include <format>
#include <vector>

bool screenshot::save(const std::string& path)
{
    int width  = 0;
    int height = 0;
    SDL_GetWindowSizeInPixels(asw::display::get_window(), &width, &height);
    if (width <= 0 || height <= 0)
    {
        return false;
    }

    std::vector<unsigned char> pixels(static_cast<size_t>(width) * height * 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    SDL_Surface* surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    if (surface == nullptr)
    {
        return false;
    }

    // GL rows start at the bottom
    const size_t rowBytes = static_cast<size_t>(width) * 4;
    for (int y = 0; y < height; y++)
    {
        const auto* from = pixels.data() + (static_cast<size_t>(height - 1 - y) * rowBytes);
        auto*       to   = static_cast<unsigned char*>(surface->pixels) + (static_cast<size_t>(y) * surface->pitch);
        std::copy_n(from, rowBytes, to);
    }

    // Opaque, so the PNG does not show the window through
    for (int y = 0; y < height; y++)
    {
        auto* row = static_cast<unsigned char*>(surface->pixels) + (static_cast<size_t>(y) * surface->pitch);
        for (int x = 0; x < width; x++)
        {
            row[(x * 4) + 3] = 255;
        }
    }

    const bool saved = IMG_SavePNG(surface, path.c_str());
    SDL_DestroySurface(surface);
    return saved;
}

std::string screenshot::saveTimestamped()
{
    const std::string folder = asw::assets::get_save_path("adsgames", "forager");
    if (folder.empty())
    {
        return "";
    }

    const auto        now  = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    const std::string path = folder + std::format("screenshot-{:%Y%m%d-%H%M%S}.png", now);
    return save(path) ? path : "";
}

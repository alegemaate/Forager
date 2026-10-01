#include "./hud.h"

namespace
{
void crosshair(GuiRenderer& gui, const glm::vec2& screen)
{
    const glm::vec2 c = screen / 2.0f;
    const glm::vec4 shadow(0.0f, 0.0f, 0.0f, 0.5f);
    const glm::vec4 white(1.0f, 1.0f, 1.0f, 0.9f);

    gui.rect(c.x - 9.0f, c.y - 2.0f, 18.0f, 4.0f, shadow);
    gui.rect(c.x - 2.0f, c.y - 9.0f, 4.0f, 18.0f, shadow);
    gui.rect(c.x - 8.0f, c.y - 1.0f, 16.0f, 2.0f, white);
    gui.rect(c.x - 1.0f, c.y - 8.0f, 2.0f, 16.0f, white);
}
} // namespace

void hud::draw(GuiRenderer& gui)
{
    crosshair(gui, gui.getSize());
}

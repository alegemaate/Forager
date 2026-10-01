#include "./menu.h"

#include <asw/asw.h>

#include "../game/controls.h"

namespace
{
constexpr f32 ROW_WIDTH  = 580.0f;
constexpr f32 ROW_HEIGHT = 44.0f;
constexpr f32 ROW_GAP    = 8.0f;
constexpr f32 TEXT_SCALE = 3.0f;

// Width of the arrow areas at the ends of a setting's value
constexpr f32 ARROW_WIDTH = 36.0f;

// Settings show their value in the right part of the row
constexpr f32 VALUE_START = 0.6f;

const glm::vec4 ROW_COLOR(0.0f, 0.0f, 0.0f, 0.55f);
const glm::vec4 FOCUS_COLOR(0.25f, 0.45f, 0.2f, 0.85f);
const glm::vec4 BORDER_COLOR(0.55f, 0.85f, 0.45f, 1.0f);
const glm::vec4 TEXT_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 VALUE_COLOR(1.0f, 0.92f, 0.6f, 1.0f);

bool contains(f32 x, f32 y, f32 w, f32 h, const glm::vec2& point)
{
    return point.x >= x && point.x < x + w && point.y >= y && point.y < y + h;
}
} // namespace

Menu::Row Menu::layout(size_t index, const glm::vec2& screen) const
{
    const f32 width = std::min(ROW_WIDTH, screen.x - 32.0f);
    return {(screen.x - width) / 2.0f, (screen.y * top) + (static_cast<f32>(index) * (ROW_HEIGHT + ROW_GAP)), width,
            ROW_HEIGHT};
}

void Menu::update()
{
    if (items.empty())
    {
        return;
    }

    if (asw::input::get_action_down(controls::UI_DOWN))
    {
        focus = (focus + 1) % items.size();
    }
    if (asw::input::get_action_down(controls::UI_UP))
    {
        focus = (focus + items.size() - 1) % items.size();
    }

    const auto& item = items[focus];
    if (item.adjust)
    {
        if (asw::input::get_action_down(controls::UI_LEFT))
        {
            item.adjust(-1);
        }
        if (asw::input::get_action_down(controls::UI_RIGHT))
        {
            item.adjust(1);
        }
    }

    if (asw::input::get_action_down(controls::UI_ACCEPT))
    {
        if (item.activate)
        {
            item.activate();
        }
        else if (item.adjust)
        {
            item.adjust(1);
        }
        return;
    }

    // Mouse
    const auto      size = asw::display::get_size();
    const glm::vec2 screen(static_cast<f32>(size.x), static_cast<f32>(size.y));
    const auto&     mouse = asw::input::get_mouse();
    const glm::vec2 point(mouse.position.x, mouse.position.y);
    const bool      moved   = mouse.change.x != 0.0f || mouse.change.y != 0.0f;
    const bool      clicked = asw::input::get_mouse_button_down(asw::input::MouseButton::Left);

    for (size_t i = 0; i < items.size(); i++)
    {
        const Row row = layout(i, screen);
        if (!contains(row.x, row.y, row.w, row.h, point))
        {
            continue;
        }

        // Hover moves the focus only when the mouse moves, so it does not fight the keys
        if (moved || clicked)
        {
            focus = i;
        }

        if (clicked)
        {
            const auto& hit = items[i];
            if (hit.adjust)
            {
                // Left arrow lowers, anywhere else raises
                const f32  arrowLeft = row.x + (row.w * VALUE_START);
                const bool lower     = point.x >= arrowLeft && point.x < arrowLeft + ARROW_WIDTH;
                hit.adjust(lower ? -1 : 1);
            }
            else if (hit.activate)
            {
                hit.activate();
            }
        }
        break;
    }
}

void Menu::draw(GuiRenderer& gui) const
{
    const glm::vec2 screen = gui.getSize();

    if (!title.empty() && !items.empty())
    {
        const Row first = layout(0, screen);
        gui.textShadow(title, screen.x / 2.0f, first.y - 56.0f, 4.0f, TEXT_COLOR, TextAlign::Center);
    }

    const f32 textOffset = (ROW_HEIGHT - GuiRenderer::textHeight(TEXT_SCALE)) / 2.0f;

    for (size_t i = 0; i < items.size(); i++)
    {
        const auto& item    = items[i];
        const Row   row     = layout(i, screen);
        const bool  focused = i == focus;

        gui.rect(row.x, row.y, row.w, row.h, focused ? FOCUS_COLOR : ROW_COLOR);
        if (focused)
        {
            gui.rectOutline(row.x, row.y, row.w, row.h, 2.0f, BORDER_COLOR);
        }

        const f32 textY = row.y + textOffset;

        if (item.value)
        {
            // Label on the left, "< value >" on the right
            gui.text(item.label, row.x + 14.0f, textY, TEXT_SCALE, TEXT_COLOR);

            const f32 valueLeft  = row.x + (row.w * VALUE_START);
            const f32 valueRight = row.x + row.w - 10.0f;
            gui.text("<", valueLeft + 8.0f, textY, TEXT_SCALE, TEXT_COLOR);
            gui.text(">", valueRight, textY, TEXT_SCALE, TEXT_COLOR, TextAlign::Right);
            gui.text(item.value(), (valueLeft + valueRight) / 2.0f + 4.0f, textY, TEXT_SCALE, VALUE_COLOR,
                     TextAlign::Center);
        }
        else
        {
            gui.text(item.label, row.x + (row.w / 2.0f), textY, TEXT_SCALE, TEXT_COLOR, TextAlign::Center);
        }
    }
}

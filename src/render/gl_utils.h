#pragma once

#include <asw/asw.h>

class GlUtils {
 public:
  /// @brief Render a filled rectangle
  /// @param rect The rectangle to fill
  /// @param color The color to fill the rectangle with
  static void rectFill(asw::Quad<float> rect, asw::Color color) {
    glBegin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(rect.x, rect.y);
    glVertex2f(rect.x + rect.w, rect.y);
    glVertex2f(rect.x + rect.w, rect.y + rect.h);
    glVertex2f(rect.x, rect.y + rect.h);
    glEnd();
  }

  static void rect(asw::Quad<float> rect, asw::Color color) {
    glBegin(GL_LINE_LOOP);
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(rect.x, rect.y);
    glVertex2f(rect.x + rect.w, rect.y);
    glVertex2f(rect.x + rect.w, rect.y + rect.h);
    glVertex2f(rect.x, rect.y + rect.h);
    glEnd();
  }

  static void line(asw::Vec2<float> start,
                   asw::Vec2<float> end,
                   asw::Color color) {
    glBegin(GL_LINES);
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(start.x, start.y);
    glVertex2f(end.x, end.y);
    glEnd();
  }
}
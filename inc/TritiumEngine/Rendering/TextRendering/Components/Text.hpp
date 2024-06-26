#pragma once

#include <memory>
#include <string>

namespace TritiumEngine::Rendering
{
  struct Font;

  class Text {
  public:
    enum class Alignment {
      TOP_LEFT,
      TOP_CENTER,
      TOP_RIGHT,
      CENTER_LEFT,
      CENTER,
      CENTER_RIGHT,
      BOTTOM_LEFT,
      BOTTOM_CENTER,
      BOTTOM_RIGHT
    };

    Text(const std::string &text, const std::string &font, float scaleFactor = 1.f,
         Alignment alignment = Alignment::BOTTOM_LEFT);
    virtual ~Text();

    float getPixelWidth() const;
    float getPixelHeight() const;
    float getMaxFontPixelHeight() const;
    Font *getFont() const { return m_font.get(); }
    unsigned int getVao() const { return m_vao; }
    unsigned int getVbo() const { return m_vbo; }

    std::string text;
    float scaleFactor;
    Alignment align;

  private:
    std::shared_ptr<Font> m_font;
    unsigned int m_vao;
    unsigned int m_vbo;
  };
} // namespace TritiumEngine::Rendering
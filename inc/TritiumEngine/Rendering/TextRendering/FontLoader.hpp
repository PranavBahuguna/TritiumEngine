#pragma once

#include <TritiumEngine/Core/ResourceLoader.hpp>
#include <TritiumEngine/Rendering/TextRendering/Font.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace TritiumEngine::Core;

namespace TritiumEngine::Rendering
{
  class FontLoader : public ResourceLoader<Font> {
  public:
    FontLoader(const std::string &defaultFilePath);
    Font *load(const std::string &filePath) override;

  private:
    bool loadFont(Font *font, const std::string &filePath, FT_Library &ft) const;
    void logFTErrorMessage(const FT_Error &error) const;

    const std::string &m_defaultFilePath;
  };
} // namespace TritiumEngine::Rendering
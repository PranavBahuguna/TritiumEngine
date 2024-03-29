#include <TritiumEngine/Rendering/ShaderLoader.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <fstream>
#include <sstream>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Rendering
{
  ShaderCode *ShaderLoader::load(const std::string &filePath) {
    std::string fileData;
    std::ifstream fileStream;
    // Allow ifstream to throw exceptions
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      std::stringstream ss;
      fileStream.open(filePath);
      ss << fileStream.rdbuf();
      fileStream.close();
      fileData = ss.str();
    } catch (std::ifstream::failure e) {
      Logger::error("[ShaderLoader] Could not read from file {}", filePath);
      return nullptr;
    }

    return new ShaderCode{fileData};
  }
} // namespace TritiumEngine::Rendering
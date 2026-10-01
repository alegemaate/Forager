/// @file screenshot.h
///
/// Save what the GL window shows
///
#pragma once

#include <string>

namespace screenshot
{

/// @brief Save the current back buffer to a PNG. Call after drawing and before the buffers swap.
/// @return true if the file was written
bool save(const std::string& path);

/// @brief Save to the game's save folder with a time stamped name
/// @return The path written, or empty on failure
std::string saveTimestamped();

} // namespace screenshot

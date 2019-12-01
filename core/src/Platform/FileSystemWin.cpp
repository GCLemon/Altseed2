#pragma once

#include <filesystem>
namespace fs = std::filesystem;

#include "FileSystem.h"

namespace altseed {
bool FileSystem::GetIsFile(const std::u16string& path) { return fs::is_regular_file(path); }

bool FileSystem::GetIsDirectory(const std::u16string& path) { return fs::is_directory(path); }

int32_t FileSystem::GetLastWriteTime(const std::u16string& path) { return fs::last_write_time(path).time_since_epoch().count(); }
}  // namespace altseed
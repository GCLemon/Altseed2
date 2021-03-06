﻿#include "File.h"

#include <zip.h>

#include <fstream>
#include <stack>

#include "../Common/StringHelper.h"
#include "../Logger/Log.h"
#include "../Platform/FileSystem.h"
#include "PackFileReader.h"

namespace Altseed {

std::shared_ptr<File> File::instance = nullptr;

bool File::Initialize(std::shared_ptr<Resources> resources) {
    instance = MakeAsdShared<File>();

    instance->m_resources = resources;

    std::lock_guard<std::mutex> lock(instance->m_rootMtx);
    // add default file root
    instance->m_roots.push_back(std::make_shared<FileRoot>(u"."));

    return true;
}

void File::Terminate() { instance = nullptr; }

std::shared_ptr<File>& File::GetInstance() { return instance; }

std::shared_ptr<BaseFileReader> File::CreateFileReader(const char16_t* path) {
    std::shared_ptr<BaseFileReader> reader = nullptr;
    std::lock_guard<std::mutex> lock(m_rootMtx);
    for (auto i = m_roots.rbegin(), e = m_roots.rend(); i != e; ++i) {
        if ((*i)->IsPack()) {
            if ((*i)->GetPackFile()->Exists(path)) {
                auto zipFile = (*i)->GetPackFile()->Load(path);
                if (zipFile == nullptr) {
                    // TODO: log failure to get zip_file
                    continue;
                }
                zip_stat_t* stat = (*i)->GetPackFile()->GetZipStat(path);
                reader = MakeAsdShared<PackFileReader>(zipFile, path, stat);
                break;
            }
        } else if (FileSystem::GetIsFile((*i)->GetPath() + path)) {
            reader = MakeAsdShared<BaseFileReader>((*i)->GetPath() + path);
            break;
        }
    }

    if (reader == nullptr && FileSystem::GetIsFile(path)) {
        reader = MakeAsdShared<BaseFileReader>(path);
    }

    return reader;
}

bool File::AddRootDirectory(const char16_t* path) {
    if (!FileSystem::GetIsDirectory(path)) {
        Log::GetInstance()->Error(LogCategory::Core, u"File::AddRootDirectory: Directory '{0}' is not found", utf16_to_utf8(path).c_str());
        return false;
    }

    std::lock_guard<std::mutex> lock(m_rootMtx);
    m_roots.push_back(std::make_shared<FileRoot>(path));
    return true;
}

bool File::AddRootPackageWithPassword(const char16_t* path, const char16_t* password) {
    if (!FileSystem::GetIsFile(path)) {
        Log::GetInstance()->Error(
                LogCategory::Core, u"File::AddRootPackageWithPassword: File '{0}' is not found", utf16_to_utf8(path).c_str());
        return false;
    }

    int error;
    zip_t* zip_ = zip_open(utf16_to_utf8(path).c_str(), ZIP_RDONLY, &error);
    if (zip_ == nullptr) {
        Log::GetInstance()->Error(
                LogCategory::Core, u"File::AddRootPackageWithPassword: Failed to open '{0}'", utf16_to_utf8(path).c_str());
        return false;
    }

    if (zip_set_default_password(zip_, utf16_to_utf8(password).c_str()) == -1) {
        zip_close(zip_);
        Log::GetInstance()->Error(
                LogCategory::Core,
                u"File::AddRootPackageWithPassword: '{0}' is invalid password for '{1}'",
                utf16_to_utf8(password).c_str(),
                utf16_to_utf8(path).c_str());
        return false;
    }

    std::lock_guard<std::mutex> lock(m_rootMtx);
    m_roots.push_back(std::make_shared<FileRoot>(path, MakeAsdShared<PackFile>(zip_, true)));
    return true;
}

bool File::AddRootPackage(const char16_t* path) {
    if (!FileSystem::GetIsFile(path)) {
        Log::GetInstance()->Error(LogCategory::Core, u"File::AddRootPackage: File '{0}' is not found", utf16_to_utf8(path).c_str());
        return false;
    }

    int error;
    zip_t* zip_ = zip_open(utf16_to_utf8(path).c_str(), ZIP_RDONLY, &error);
    if (zip_ == nullptr) {
        Log::GetInstance()->Error(LogCategory::Core, u"File::AddRootPackage: Failed to open '{0}'", utf16_to_utf8(path).c_str());
        return false;
    }

    std::lock_guard<std::mutex> lock(m_rootMtx);
    m_roots.push_back(std::make_shared<FileRoot>(path, MakeAsdShared<PackFile>(zip_)));
    return true;
}

void File::ClearRootDirectories() {
    std::lock_guard<std::mutex> lock(m_rootMtx);
    m_roots.clear();

    // add default file root
    m_roots.push_back(std::make_shared<FileRoot>(u"."));
}

bool File::Exists(const char16_t* path) const {
    for (auto i = m_roots.rbegin(), e = m_roots.rend(); i != e; ++i) {
        if ((*i)->IsPack()) {
            if ((*i)->GetPackFile()->Exists(path)) return true;
        } else if (FileSystem::GetIsFile((*i)->GetPath() + path))
            return true;
    }

    if (FileSystem::GetIsFile(path)) return true;
    return false;
}

bool File::Pack(const char16_t* srcPath, const char16_t* dstPath) const {
    if (!FileSystem::GetIsDirectory(srcPath)) {
        Log::GetInstance()->Error(LogCategory::Core, u"File::Pack: Directory '{0}' is not found", utf16_to_utf8(srcPath).c_str());
        return false;
    }

    int error;
    zip_t* zip_ = zip_open(utf16_to_utf8(dstPath).c_str(), ZIP_TRUNCATE | ZIP_CREATE, &error);
    if (zip_ == nullptr) {
        Log::GetInstance()->Error(LogCategory::Core, u"File::Pack: Failed to create '{0}'", utf16_to_utf8(dstPath).c_str());
        return false;
    }

    auto res = MakePackage(zip_, srcPath);
    zip_close(zip_);
    if (!res) {
        Log::GetInstance()->Error(LogCategory::Core, u"File::PackWithPassword: Filed to MakePackage '{0}'", utf16_to_utf8(srcPath).c_str());
    }
    return res;
}

bool File::PackWithPassword(const char16_t* srcPath, const char16_t* dstPath, const char16_t* password) const {
    if (!FileSystem::GetIsDirectory(srcPath)) {
        Log::GetInstance()->Error(
                LogCategory::Core, u"File::PackWithPassword: Directory '{0}' is not found", utf16_to_utf8(srcPath).c_str());
        return false;
    }

    int error;
    zip_t* zip_ = zip_open(utf16_to_utf8(dstPath).c_str(), ZIP_TRUNCATE | ZIP_CREATE, &error);
    if (zip_ == nullptr) {
        Log::GetInstance()->Error(LogCategory::Core, u"File::PackWithPassword: Filed to create '{0}'", utf16_to_utf8(dstPath).c_str());
        return false;
    }

    if (zip_set_default_password(zip_, utf16_to_utf8(password).c_str()) == -1) {
        zip_close(zip_);
        Log::GetInstance()->Error(
                LogCategory::Core,
                u"File::PackWithPassword: '{0}' is invalid password for '{1}'",
                utf16_to_utf8(password).c_str(),
                utf16_to_utf8(dstPath).c_str());
        return false;
    }

    auto res = MakePackage(zip_, srcPath, true);
    zip_close(zip_);
    if (!res) {
        Log::GetInstance()->Error(LogCategory::Core, u"File::PackWithPassword: Filed to MakePackage '{0}'", utf16_to_utf8(srcPath).c_str());
    }
    return res;
}

bool File::MakePackage(zip_t* zipPtr, const std::u16string& path, bool isEncrypt) const {
    std::stack<std::u16string> children;

    std::u16string current;
    children.push(path);

    while (!children.empty()) {
        current = children.top();
        children.pop();

        std::vector<std::u16string> paths;
        FileSystem::GetChildPaths(current, paths);
        for (auto& i : paths) {
            std::u16string zipPath = i;
            zipPath.erase(0, path.size());

            if (FileSystem::GetIsDirectory(i)) {
                if (zip_dir_add(zipPtr, utf16_to_utf8(zipPath).c_str(), ZIP_FL_ENC_UTF_8) == -1) return false;
                children.push(i);
            } else if (FileSystem::GetIsFile(i)) {
#if _WIN32
                std::ifstream file((wchar_t*)i.c_str(), std::ios::binary);
#else
                std::ifstream file(utf16_to_utf8(i).c_str(), std::ios::binary);
#endif
                if (!file.is_open()) return false;

                int size = FileSystem::GetFileSize(i);

                // libzip requires malloced buffer (not new)
                char* buffer = static_cast<char*>(malloc(size));
                file.read(buffer, size);
                zip_source_t* zipSource;
                zip_error error;
                if ((zipSource = zip_source_buffer_create(buffer, size, 1, &error)) == nullptr) return false;

                zip_int64_t index;
                if ((index = zip_file_add(zipPtr, utf16_to_utf8(zipPath).c_str(), zipSource, ZIP_FL_ENC_UTF_8)) == -1) {
                    zip_source_free(zipSource);
                    return false;
                }
                if (isEncrypt && zip_file_set_encryption(zipPtr, index, ZIP_EM_AES_256, nullptr) == -1) {
                    zip_source_free(zipSource);
                    return false;
                }
            }
        }
    }

    return true;
}

}  // namespace Altseed
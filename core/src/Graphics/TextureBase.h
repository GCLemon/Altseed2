#pragma once

#include <LLGI.Base.h>

#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "../Common/Resource.h"
#include "../Common/Resources.h"
#include "../Common/ThreadSafeMap.h"
#include "../Math/Vector2I.h"

namespace Altseed {
class TextureBase : public Resource {
private:
    static ThreadSafeMap<std::u16string, std::mutex> mtxs;

protected:
    std::shared_ptr<Resources> resources_ = nullptr;
    std::shared_ptr<LLGI::Texture> m_texture = nullptr;
    Vector2I size_;

    TextureBase(){};

public:
    TextureBase(std::shared_ptr<Resources>& resources, std::shared_ptr<LLGI::Texture>& texture);
    virtual ~TextureBase();

    Vector2I GetSize() const;

    bool Save(const char16_t* path);

    std::shared_ptr<LLGI::Texture>& GetNativeTexture() { return m_texture; }
};
}  // namespace Altseed

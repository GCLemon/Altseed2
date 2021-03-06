﻿#include "RenderTexture.h"

#include "Graphics.h"
#include "Texture2D.h"

namespace Altseed {

RenderTexture::RenderTexture(const std::shared_ptr<LLGI::Texture>& texture) { m_texture = texture; }

RenderTexture::RenderTexture(Vector2I size) {
    m_texture = Graphics::GetInstance()->CreateRenderTexture(size.X, size.Y);
    size_.X = m_texture->GetSizeAs2D().X;
    size_.Y = m_texture->GetSizeAs2D().Y;
}

RenderTexture::~RenderTexture() { m_texture = nullptr; }

std::shared_ptr<RenderTexture> RenderTexture::Create(Vector2I size) { return MakeAsdShared<RenderTexture>(size); }

}  // namespace Altseed
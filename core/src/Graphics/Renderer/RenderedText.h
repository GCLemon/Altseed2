#pragma once

#include "Rendered.h"

#include <box2d/box2d.h>
#include <stdio.h>
#include <memory>
#include "../Color.h"
#include "../Font.h"
#include "../Material.h"
#include "CullingSystem.h"

namespace Altseed {

class RenderedText : public Rendered {
private:
    std::shared_ptr<Material> material_;
    std::shared_ptr<Font> font_;
    std::u16string text_;
    Color color_;
    float weight_;

public:
    static std::shared_ptr<RenderedText> Create();

    float GetWeight() const { return weight_; }

    void SetWeight(float weight) { weight_ = weight; }

    std::shared_ptr<Material> GetMaterial() const { return material_; }

    void SetMaterial(const std::shared_ptr<Material>& material) { material_ = material; }

    std::shared_ptr<Font> GetFont() const { return font_; }

    void SetFont(const std::shared_ptr<Font>& font) {
        font_ = font;
        cullingSystem_->RequestUpdateAABB(this);
    }

    const char16_t* GetText() const { return text_.c_str(); }

    //! Internal function
    const std::u16string& GetTextAsStr() const { return text_; }

    void SetText(const char16_t* text) {
        text_ = std::u16string(text);
        cullingSystem_->RequestUpdateAABB(this);
    }

    Color GetColor() const { return color_; }

    void SetColor(const Color color) { color_ = color; }

    Vector2F CalcTextureSize();

    b2AABB GetAABB() override;
};

}  // namespace Altseed
/*
 * Copyright 2019 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkSGRenderEffect_DEFINED
#define SkSGRenderEffect_DEFINED

#include "SkSGEffectNode.h"

#include "SkBlendMode.h"
#include "SkColor.h"

#include <memory>
#include <vector>

// TODO: merge EffectNode.h with this header

class SkImageFilter;

namespace sksg {

/**
 * ImageFilter base class.
 */
class ImageFilter : public Node {
public:
    ~ImageFilter() override;

    const sk_sp<SkImageFilter>& getFilter() const {
        SkASSERT(!this->hasInval());
        return fFilter;
    }

protected:
    explicit ImageFilter(sk_sp<ImageFilter> input = 0);

    using InputsT = std::vector<sk_sp<ImageFilter>>;
    explicit ImageFilter(std::unique_ptr<InputsT> inputs);

    SkRect onRevalidate(InvalidationController*, const SkMatrix&) final;

    virtual sk_sp<SkImageFilter> onRevalidateFilter() = 0;

    sk_sp<SkImageFilter> refInput(size_t) const;

private:
    const std::unique_ptr<InputsT> fInputs;

    sk_sp<SkImageFilter>           fFilter;

    using INHERITED = Node;
};

/**
 * Attaches an ImageFilter (chain) to the render DAG.
 */
class ImageFilterEffect final : public EffectNode {
public:
    ~ImageFilterEffect() override;

    static sk_sp<RenderNode> Make(sk_sp<RenderNode> child, sk_sp<ImageFilter> filter);

protected:
    void onRender(SkCanvas*, const RenderContext*) const override;
    const RenderNode* onNodeAt(const SkPoint&)     const override;

    SkRect onRevalidate(InvalidationController*, const SkMatrix&) override;

private:
    ImageFilterEffect(sk_sp<RenderNode> child, sk_sp<ImageFilter> filter);

    sk_sp<ImageFilter> fImageFilter;

    using INHERITED = EffectNode;
};

/**
 * SkDropShadowImageFilter node.
 */
class DropShadowImageFilter final : public ImageFilter {
public:
    ~DropShadowImageFilter() override;

    static sk_sp<DropShadowImageFilter> Make(sk_sp<ImageFilter> input = nullptr);

    enum class Mode { kShadowAndForeground, kShadowOnly };

    SG_ATTRIBUTE(Offset, SkVector, fOffset)
    SG_ATTRIBUTE(Sigma , SkVector, fSigma )
    SG_ATTRIBUTE(Color , SkColor , fColor )
    SG_ATTRIBUTE(Mode  , Mode    , fMode  )

protected:
    sk_sp<SkImageFilter> onRevalidateFilter() override;

private:
    explicit DropShadowImageFilter(sk_sp<ImageFilter> input);

    SkVector             fOffset = { 0, 0 },
                         fSigma  = { 0, 0 };
    SkColor              fColor  = SK_ColorBLACK;
    Mode                 fMode   = Mode::kShadowAndForeground;

    using INHERITED = ImageFilter;
};

/**
 * Applies a SkBlendMode to descendant render nodes.
 */
class BlendModeEffect final : public EffectNode {
public:
    ~BlendModeEffect() override;

    static sk_sp<BlendModeEffect> Make(sk_sp<RenderNode> child,
                                       SkBlendMode = SkBlendMode::kSrcOver);

    SG_ATTRIBUTE(Mode, SkBlendMode, fMode)

protected:
    void onRender(SkCanvas*, const RenderContext*) const override;
    const RenderNode* onNodeAt(const SkPoint&)     const override;

private:
    BlendModeEffect(sk_sp<RenderNode>, SkBlendMode);

    SkBlendMode fMode;

    using INHERITED = EffectNode;
};

} // namespace sksg

#endif // SkSGRenderEffect_DEFINED

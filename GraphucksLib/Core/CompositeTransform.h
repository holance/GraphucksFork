#pragma once
#include "Transform.h"

namespace Graphucks
{
    class CompositeTransform;
    typedef std::shared_ptr<CompositeTransform> CompositeTransformPtr;

    class CompositeTransform : public Transform
    {
    public:
        CompositeTransform(void);
        virtual ~CompositeTransform(void);
    public:
        auto CenterX() const -> float;
        void CenterX(float val);

        auto CenterY() const -> float;
        void CenterY(float val);

        auto ScaleX() const -> float;
        void ScaleX(float val);

        auto ScaleY() const -> float;
        void ScaleY(float val);

        auto SkewX() const -> float;
        void SkewX(float val);

        auto SkewY() const -> float;
        void SkewY(float val);

        auto TranslateX() const -> float;
        void TranslateX(float val);

        auto TranslateY() const -> float;
        void TranslateY(float val);

        auto Rotation() const -> float;
        void Rotation(float val);

        virtual operator D2D1::Matrix3x2F() override;
        virtual auto GetTransformMatrix() -> D2D1::Matrix3x2F&;
    private:
        void Calculate();
    private:
        bool             m_isDirty;
        float            m_centerX;
        float            m_centerY;
        float            m_scaleX;
        float            m_scaleY;
        float            m_skewX;
        float            m_skewY;
        float            m_translateX;
        float            m_translateY;
        float            m_rotation;
        D2D1::Matrix3x2F m_calculatedMatrix;
    };
}

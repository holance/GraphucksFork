#pragma once
#include "Transform.h"

namespace Graphucks
{
    class MatrixTransform;
    typedef std::shared_ptr<MatrixTransform> MatrixTransformPtr;

    class MatrixTransform : public Transform
    {
    public:
        MatrixTransform(void);
        virtual ~MatrixTransform(void);
    public:
        virtual operator D2D1::Matrix3x2F() override;
        virtual auto GetTransformMatrix() -> D2D1::Matrix3x2F&;
    public:
        float M11();
        void M11(const float& val);

        float M12();
        void M12(const float& val);

        float M21();
        void M21(const float& val);

        float M22();
        void M22(const float& val);

        float OffsetX();
        void OffsetX(const float& val);

        float OffsetY();
        void OffsetY(const float& val);
    private:
        bool             m_isDirty;
        D2D1::Matrix3x2F m_matrix;
    };
}

#pragma once

namespace sraphy
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void Initialize(int32_t p_Width, int32_t pHeight);

        uint32_t GetFrameBufferHandle();
        uint32_t GetFrameBufferTextureHandle();

        void Bind();
        void Unbind();

        void Clear();

    private:
        uint32_t m_Id;
        uint32_t m_DepthRbo;
        uint32_t m_ColorAttachmentTexture;

        std::array<float, 4> m_ClearRGBA;
        std::array<float, 1> m_ClearDepth;
    };
} // namespace sraphy

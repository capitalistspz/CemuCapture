#ifndef CEMU_CAPTURE_SOURCE_COMMON_HPP
#define CEMU_CAPTURE_SOURCE_COMMON_HPP
#include "CemuCapture.hpp"

namespace CemuCapture
{
    class SourceCommon : public Source
    {
        std::function<void(Source&, CaptureErrorType, std::span<const std::uint8_t>)> m_captureFun;
        CaptureErrorPolicy m_frameErrorPolicy{};
        ImageFormat m_outputImageFormat{ImageFormat::Unspecified};
        unsigned m_outputStride{0};
    public:
        void SetCaptureCallback(std::function<void(Source&, CaptureErrorType, std::span<const uint8_t> data)> fn) override
        {
            m_captureFun = std::move(fn);
        }
        void InvokeCaptureCallback(CaptureErrorType errorType, std::span<const std::uint8_t> data)
        {
            if (m_captureFun)
                m_captureFun(*this, errorType, data);
        }

        void SetCaptureErrorPolicy(CaptureErrorPolicy policy) override
        {
            m_frameErrorPolicy = policy;
        }

        void SetOutputFormat(ImageFormat imageFormat, unsigned stride) override
        {
            m_outputImageFormat = imageFormat;
            m_outputStride = stride;
        }

        [[nodiscard]] ImageFormat GetOutputImageFormat() const noexcept
        {
            return m_outputImageFormat;
        }

        [[nodiscard]] unsigned GetOutputStride() const noexcept
        {
            return m_outputStride;
        }

        [[nodiscard]] CaptureErrorPolicy GetFrameErrorPolicy() const noexcept
        {
            return m_frameErrorPolicy;
        }

        ~SourceCommon() override = default;
    };
}
#endif

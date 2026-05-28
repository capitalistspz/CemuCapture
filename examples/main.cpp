#include <cassert>
#include <chrono>
#include <fstream>
#include <libyuv.h>
#include <CemuCapture.hpp>
#include <print>
#include <thread>

void LogCallback(CemuCapture::LogLevel, std::string_view);
std::string_view FormatToString(CemuCapture::ImageFormat);

int main()
{
    auto context = CemuCapture::Context::Create();
    context->SetLogCallback(LogCallback);

    const auto sourceInfos = context->EnumerateSources();
    if (sourceInfos.empty())
    {
        std::println("No devices found");
        return 1;
    }

    for (const auto& info : sourceInfos)
    {
        std::println("{} {}", info.id, info.name);
    }

    auto source = context->OpenDevice(sourceInfos.front().id);
    for (const auto& [dimensions, framerate, format] : source->EnumerateStreamFormats())
    {
        std::println("{} {}x{}@{} fps", FormatToString(format), dimensions.width, dimensions.height, framerate);
    }
    source->SetOutputFormat(CemuCapture::ImageFormat::NV12, 0);
    source->SetCaptureErrorPolicy(CemuCapture::CaptureErrorPolicy::PushBadFrame);

    const auto actualFormat = source->StartStreaming({
        .dimensions = {640, 480}, .framerate = 30, .format = CemuCapture::ImageFormat::YUYV
    });
    if (!actualFormat)
        return 1;
    auto rgbBuffer = std::vector<uint8_t>(actualFormat->dimensions.width * actualFormat->dimensions.height * 3);

    source->SetCaptureCallback(
        [&](CemuCapture::Source&, CemuCapture::CaptureErrorType type, std::span<const std::uint8_t> bytes)
        {
            static auto counter = 0u;
            static auto lastTime = std::chrono::high_resolution_clock::now();
            const auto dims = actualFormat->dimensions;
            const auto planeSize = dims.width * dims.height;

            const auto fileName = std::format("output_{}x{}_{}{}.bgr", dims.width, dims.height, counter++,
                                        type == CemuCapture::CaptureErrorType::None ? "" : "_bad");
            if (auto file = std::ofstream(fileName))
            {
                // Convert to BGR888
                libyuv::NV12ToRGB24(bytes.data(), dims.width, bytes.data() + planeSize, dims.width,
                                    rgbBuffer.data(),
                                    dims.width * 3, dims.width, dims.height);
                file.write(reinterpret_cast<const std::ostream::char_type*>(rgbBuffer.data()), rgbBuffer.size());
            }


            const auto now = std::chrono::high_resolution_clock::now();
            std::println("{}ms", std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count());
            lastTime = now;
        });

    std::this_thread::sleep_for(std::chrono::seconds(30));
}

void LogCallback(CemuCapture::LogLevel level, std::string_view sv)
{
    switch (level)
    {
    case CemuCapture::LogLevel::Info:
        std::print("[Info] ");
        break;
    case CemuCapture::LogLevel::Warning:
        std::print("[Warn] ");
        break;
    case CemuCapture::LogLevel::Error:
        std::print("[Err ] ");
        break;
    default:
        assert(false);
    }
    std::println("{}", sv);
}

std::string_view FormatToString(CemuCapture::ImageFormat format)
{
    switch (format)
    {
    case CemuCapture::ImageFormat::Unspecified:
        return "None";
    case CemuCapture::ImageFormat::NV12:
        return "NV12";
    case CemuCapture::ImageFormat::NV21:
        return "NV21";
    case CemuCapture::ImageFormat::YUYV:
        return "YUYV";
    case CemuCapture::ImageFormat::UYVY:
        return "UYVY";
    case CemuCapture::ImageFormat::MJPG:
        return "MJPEG";
    case CemuCapture::ImageFormat::RGB24:
        return "RGB24";
    case CemuCapture::ImageFormat::ARGB32:
        return "ARGB32";
    default:
        throw std::invalid_argument("Invalid format");
    }
}

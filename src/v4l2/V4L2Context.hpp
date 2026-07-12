#ifndef CEMU_CAPTURE_V4L2_CONTEXT_HPP
#define CEMU_CAPTURE_V4L2_CONTEXT_HPP
#include <condition_variable>
#include <thread>

#include "CemuCapture.hpp"
#include "../ContextCommon.hpp"
#include "FileDescriptor.hpp"

struct epoll_event;

#ifdef CEMU_CAPTURE_ENUMERATE_WITH_LIBUDEV
struct udev;
extern "C" udev* udev_unref(udev*);
using udev_ptr = std::unique_ptr<udev, decltype(&udev_unref)>;
#endif

namespace CemuCapture
{
    class V4L2Source;

    class V4L2Context final : public ContextCommon, public std::enable_shared_from_this<V4L2Context>
    {
        std::optional<SourceInfo> GetDeviceInfo(FileDescriptor& fd, std::string_view deviceFilePath);
        void ThreadFunc(std::stop_token);

    public:
        V4L2Context();
        ~V4L2Context() override;
        std::shared_ptr<Source> OpenDevice(std::string const& id) override;
        std::vector<SourceInfo> EnumerateSources() override;
        void AddDevice(const std::shared_ptr<V4L2Source>&);

    private:
        std::condition_variable m_eventCond;
        std::mutex m_eventMutex;
        std::vector<epoll_event> m_newEvents;
        std::jthread m_thread;
        FileDescriptor m_epollFd;
#ifdef CEMU_CAPTURE_ENUMERATE_WITH_LIBUDEV
        udev_ptr m_udev;
#endif
    };
}

#endif

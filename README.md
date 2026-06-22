# CemuCapture
A cross-platform camera capture library supporting Linux (via V4L2) and Windows (via MediaFoundation).

## Usage

In `CMakeLists.txt`
```cmake
include(FetchContent)

FetchContent_Declare(CemuCapture
    GIT_REPOSITORY https://github.com/capitalistspz/CemuCapture
)
FetchContent_MakeAvailable(CemuCapture)

add_executable(my_executable main.cpp)

target_link_libraries(my_executable PRIVATE CemuCapture::CemuCapture)
```

### CMake options
- `CEMU_CAPTURE_USE_LIBSYSTEMD` (Linux only)
- `CEMU_CAPTURE_BUILD_EXAMPLES`

### Dependencies: 
- [libyuv](https://chromium.googlesource.com/libyuv/libyuv/)
- [libsystemd](https://www.freedesktop.org/software/systemd/man/latest/libsystemd.html) (Linux only, optional)
- [WIL](https://github.com/microsoft/wil) (Windows only)

### Notes
Does not compile with MINGW, WIL only supports MSVC and Clang CL.
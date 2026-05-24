cmake_minimum_required(VERSION 3.23)
project(CemuCapture CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CMakeDependentOption)

cmake_dependent_option(CEMU_CAPTURE_USE_LIBSYSTEMD "Use systemd for device enumeration" ON "UNIX AND NOT APPLE" OFF)

option(CEMU_CAPTURE_BUILD_EXAMPLES "Build examples" ${PROJECT_IS_TOP_LEVEL})

if (CEMU_CAPTURE_USE_LIBSYSTEMD)
    find_package(libsystemd REQUIRED)
endif ()

if (WIN32)
    find_package(WIL REQUIRED)

    find_package(Boost QUIET COMPONENTS nowide)
    if (NOT Boost_NOWIDE_FOUND)
        find_package(nowide REQUIRED)
        set(CemuCapture_nowide_standalone TRUE)
    else()
        set(CemuCapture_nowide_standlone FALSE)
    endif()
endif()

# VCPKG sets CMAKE_FIND_PACKAGE_PREFER_CONFIG, but doesn't provide libyv::libyuv as a target, so the find module adds that alias
find_package(libyuv MODULE REQUIRED)

add_library(CemuCapture src/ContextCommon.hpp src/SourceCommon.hpp src/util.hpp)
add_library(CemuCapture::CemuCapture ALIAS CemuCapture)

target_compile_features(CemuCapture PUBLIC cxx_std_20)

target_sources(CemuCapture
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES include/CemuCapture.hpp
)

set_target_properties(CemuCapture PROPERTIES VERIFY_INTERFACE_HEADER_SETS TRUE)

if (UNIX AND NOT APPLE)
    target_sources(CemuCapture PRIVATE
        src/v4l2/ConversionTable.hpp
        src/v4l2/FileDescriptor.hpp
        src/v4l2/Fract.hpp
        src/v4l2/V4L2Ioctls.hpp
        src/v4l2/MemoryMapped.hpp
        src/v4l2/V4L2Context.cpp
        src/v4l2/V4L2Context.hpp
        src/v4l2/V4L2Source.cpp
        src/v4l2/V4L2Source.hpp
    )
    message(STATUS "CemuCapture: V4L2 backend")
elseif (WIN32)
    target_sources(CemuCapture PRIVATE
        src/mf/MFContext.cpp
        src/mf/MFContext.hpp
        src/mf/MFSource.cpp
        src/mf/MFSource.hpp
        src/mf/Conversion.hpp
        src/mf/Conversion.cpp
        src/mf/Common.hpp
    )
    target_compile_definitions(CemuCapture PRIVATE UNICODE _UNICODE)
    target_link_libraries(CemuCapture PRIVATE WIL::WIL mf mfplat mfreadwrite mfuuid shlwapi)
    if (CemuCapture_nowide_standalone)
        target_link_libraries(CemuCapture PRIVATE nowide::nowide)
        target_compile_definitions(CemuCapture PRIVATE CEMU_CAPTURE_NOWIDE_STANDLONE)
        message(STATUS "CemuCapture: Using standalone nowide")
    else()
        target_link_libraries(CemuCapture PRIVATE Boost::nowide)
    endif()
    message(STATUS "CemuCapture: MediaFoundation backend")
else()
    message(SEND_ERROR "Unsupported platform")
endif ()

target_link_libraries(CemuCapture PRIVATE libyuv::libyuv)

if (CEMU_CAPTURE_USE_LIBSYSTEMD)
    target_link_libraries(CemuCapture PRIVATE libsystemd::libsystemd)
    target_compile_definitions(CemuCapture PRIVATE CEMU_CAPTURE_USE_LIBSYSTEMD)
endif ()

if (CEMU_CAPTURE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

include(GNUInstallDirs)

install(TARGETS CemuCapture
        EXPORT CemuCapture-targets
        FILE_SET HEADERS
)

install(EXPORT CemuCapture-targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CemuCapture
        NAMESPACE CemuCapture::
)
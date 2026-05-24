
add_executable(example main.cpp)

target_link_libraries(example PRIVATE CemuCapture::CemuCapture libyuv::libyuv)
target_compile_features(example PRIVATE cxx_std_23)
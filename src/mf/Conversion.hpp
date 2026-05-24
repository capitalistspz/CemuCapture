#ifndef CEMU_CAPTURE_MF_CONVERSION_HPP
#define CEMU_CAPTURE_MF_CONVERSION_HPP
#include <mfobjects.h>
#include <vector>

#include "CemuCapture.hpp"

namespace CemuCapture::conversion
{
	bool CanConvert(ImageFormat in, ImageFormat out);
	void Convert(IMFMediaBuffer& buffer, ImageFormat inputFormat, ImageFormat outputFormat, unsigned outputStride, const Dimensions& dims, std::vector<uint8_t>& output);
} // namespace CemuCapture::conversion
#endif
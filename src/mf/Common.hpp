#ifndef CEMU_CAPTURE_MF_COMMON_HPP
#define CEMU_CAPTURE_MF_COMMON_HPP
#include <winerror.h>
#include <stringapiset.h>

#if defined(_MSC_VER)
#define DEBUG_BREAK __debugbreak()
#else
#define DEBUG_BREAK
#endif

namespace CemuCapture
{
	inline void assert_hres_eval(HRESULT hres)
	{
#ifndef NDEBUG
		if (FAILED(hres))
		{
			DEBUG_BREAK;
		}
#endif
	}

	inline std::string narrow(std::wstring_view str)
	{
		if (str.empty())
			return {};
		const auto utf8Length = WideCharToMultiByte(
			CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0, nullptr, nullptr);

		std::string narrowStr(utf8Length, '\0');
		WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), narrowStr.data(), utf8Length, nullptr, nullptr);
		return narrowStr;
	}

	inline std::wstring widen(std::string_view str)
	{
		if (str.empty())
			return {};
		const auto wideLength = MultiByteToWideChar(
			CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);

		std::wstring narrowStr(wideLength, '\0');
		MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), narrowStr.data(), wideLength);
		return narrowStr;
	}
} // namespace CemuCapture
#endif
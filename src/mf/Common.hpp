#ifndef CEMU_CAPTURE_MF_COMMON_HPP
#define CEMU_CAPTURE_MF_COMMON_HPP
#ifndef CEMU_CAPTURE_NOWIDE_STANDLONE
#include <boost/nowide/convert.hpp>
namespace nowide = boost::nowide;
#else
#include <nowide/convert.hpp>
#endif
#include <winerror.h>

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
} // namespace CemuCapture
#endif
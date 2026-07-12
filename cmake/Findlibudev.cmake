
find_package(libudev CONFIG QUIET)
if (NOT libudev_FOUND)
    find_package(PkgConfig QUIET)
    if (PKG_CONFIG_FOUND)
        pkg_search_module(libudev QUIET IMPORTED_TARGET libudev)
        if (libudev_FOUND)
            add_library(libudev::libudev ALIAS PkgConfig::libudev)
        endif ()
    endif ()
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libudev
    REQUIRED_VARS
    libudev_FOUND)
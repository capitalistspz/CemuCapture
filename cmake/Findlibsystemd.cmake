
find_package(libsystemd CONFIG QUIET)
if (NOT libsystemd_FOUND)
    find_package(PkgConfig QUIET)
    if (PKG_CONFIG_FOUND)
        pkg_search_module(libsystemd QUIET IMPORTED_TARGET libsystemd)
        if (libsystemd_FOUND)
            add_library(libsystemd::libsystemd ALIAS PkgConfig::libsystemd)
        endif ()
    endif ()
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libsystemd
        REQUIRED_VARS
        libsystemd_FOUND)
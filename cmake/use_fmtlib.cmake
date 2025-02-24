if (NOT FORCE_BUNDLED_FMT)
    find_package(fmt QUIET)
endif (NOT FORCE_BUNDLED_FMT)

if (NOT TARGET fmt::fmt)
    message(STATUS "Using bundled libfmt")
    set(FMT_DOC OFF CACHE BOOL "FMT Doc overwritten")
    set(FMT_FUZZ OFF CACHE BOOL "FMT Fuzz overwritten")
    set(FMT_INSTALL OFF CACHE BOOL "FMT Install overwritten")
    set(FMT_PEDANTIC ON CACHE BOOL "FMT Pedantic overwritten")
    set(FMT_TEST OFF CACHE BOOL "FMT Test overwritten")
    set(FMT_WERROR OFF CACHE BOOL "FMT WError overwritten")

    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/fmt")
    mark_as_advanced(FORCE FMT_DOC FMT_FUZZ FMT_INSTALL FMT_PEDANTIC FMT_TEST
                           FMT_WERROR FMT_CUDA_TEST fmt_DIR)
else ()
    message(STATUS "Found libfmt in the system")
endif (NOT TARGET fmt::fmt)

if (NOT FORCE_BUNDLED_EIGEN)
    find_package(Eigen3 3.3 QUIET)
endif (NOT FORCE_BUNDLED_EIGEN)

if (NOT Eigen3_FOUND)
    set(CUSTOM_EIGEN_SOURCE "" CACHE PATH
        "Custom directory for Eigen3 source code in system")

    if (CUSTOM_EIGEN_SOURCE)
        set(EIGEN_SOURCE ${CUSTOM_EIGEN_SOURCE})
        set(EIGEN_URL "")
    else ()
        set(EIGEN_VERSION "3.3.7")
        message(STATUS "Using bundled Eigen3 - Version ${EIGEN_VERSION}")
        set(EIGEN_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/third_party/eigen3/src")
        set(EIGEN_URL
            "https://gitlab.com/libeigen/eigen/-/archive/${EIGEN_VERSION}/eigen-${EIGEN_VERSION}.tar.gz")
    endif (CUSTOM_EIGEN_SOURCE)

    include(ExternalProject)

    ExternalProject_Add(eigen3
      PREFIX "${CMAKE_CURRENT_BINARY_DIR}/third_party/eigen3"
      STAMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/third_party/eigen3/stamp"
      URL ${EIGEN_URL}

      SOURCE_DIR ${EIGEN_SOURCE}
      CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/third_party/eigen3-install"

      BUILD_COMMAND ""
      BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/third_party/eigen3/build"

      INSTALL_DIR ""

      # Output logging
      LOG_DOWNLOAD 1
      LOG_UPDATE 0
      LOG_CONFIGURE 1
      LOG_BUILD 0
      LOG_TEST 0
      LOG_INSTALL 1
    )
    set_target_properties(eigen3 PROPERTIES EXCLUDE_FROM_ALL TRUE)

    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/third_party/eigen3-install")
        set(eigen3_built TRUE)
        find_package(Eigen3 3.3 REQUIRED
                     PATHS "${CMAKE_CURRENT_BINARY_DIR}/third_party/eigen3-install/"
                     NO_DEFAULT_PATH)
        # Hide this variable in cmake-gui.
        mark_as_advanced(FORCE Eigen3_DIR)
    else ()
        set(eigen3_built FALSE)
    endif (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/third_party/eigen3-install")
else ()
    message(STATUS "Found Eigen3 as system library")
    mark_as_advanced(FORCE Eigen3_DIR)
    add_custom_target(eigen3)
    set(eigen3_built TRUE)
endif (NOT Eigen3_FOUND)

message(STATUS "Using Eigen from ${Eigen3_DIR}")

if (NOT FORCE_BUNDLED_CV)
    find_package(OpenCV 4 QUIET)
endif (NOT FORCE_BUNDLED_CV)

if (NOT OpenCV_FOUND)
    set(CUSTOM_OPENCV_SOURCE "" CACHE PATH
        "Custom directory for downloaded opencv source code")
    set(CUSTOM_OPENCV_CONTRIB_SOURCE "" CACHE PATH
        "Custom directory for downloaded opencv contrib modules source code")

    if (CUSTOM_OPENCV_SOURCE)
        set(DOWNLOAD_SEQUENCE sh -c "echo Not downloading!")
        set(OPENCV_SOURCE ${CUSTOM_OPENCV_SOURCE})
        set(OPENCV_CONTRIB_MODULES ${CUSTOM_OPENCV_CONTRIB_SOURCE})
        message(STATUS "Use predownloaded OpenCV paths! => ${CUSTOM_OPENCV_SOURCE} ; ${CUSTOM_OPENCV_CONTRIB_SOURCE}")
    else ()
        set(CV_VERSION "4.3.0")
        message(STATUS "Using bundled OpenCV Library - Version ${CV_VERSION}")
        set(DOWNLOAD_SEQUENCE sh -c "wget \
            https://github.com/opencv/opencv/archive/${CV_VERSION}.zip \
            && unzip -o ${CV_VERSION}.zip \
            && rsync -a opencv-${CV_VERSION}/ ./ \
            && rm -r opencv-${CV_VERSION} ${CV_VERSION}.zip \
            && wget \
            https://github.com/opencv/opencv_contrib/archive/${CV_VERSION}.zip \
            && unzip -o ${CV_VERSION}.zip \
            && mv opencv_contrib-${CV_VERSION} opencv_contrib \
            && rm ${CV_VERSION}.zip")

        set(OPENCV_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/third_party/opencv/src")
        set(OPENCV_CONTRIB_MODULES
            "${CMAKE_CURRENT_BINARY_DIR}/third_party/opencv/src/opencv_contrib/modules")
    endif (CUSTOM_OPENCV_SOURCE)

    include(opencv_options)
    include(ExternalProject)

    ExternalProject_Add(opencv
      PREFIX "${CMAKE_CURRENT_BINARY_DIR}/third_party/opencv"
      DOWNLOAD_COMMAND ${DOWNLOAD_SEQUENCE}
      SOURCE_DIR ${OPENCV_SOURCE}
      CMAKE_ARGS "${opencv_options};-DCMAKE_INSTALL_PREFIX=/usr"

      # Build step
      BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/third_party/opencv-build"

      # Install step
      INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/third_party/opencv-install"
      INSTALL_COMMAND sh -c "DESTDIR=${CMAKE_BINARY_DIR}/third_party/opencv-install ${CMAKE_MAKE_PROGRAM} install"

      # Test step
      TEST_BEFORE_INSTALL 0
      TEST_AFTER_INSTALL 0
      # TEST_COMMAND "${CMAKE_MAKE_PROGRAM} test"

      # Output logging
      LOG_DOWNLOAD 1
      LOG_UPDATE 0
      LOG_CONFIGURE 1
      LOG_BUILD 1
      LOG_TEST 1
      LOG_INSTALL 1
    )
    set_target_properties(opencv PROPERTIES EXCLUDE_FROM_ALL TRUE)

    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/third_party/opencv-install/usr")
        set(opencv_built TRUE)
        find_package(OpenCV REQUIRED
                     PATHS "${CMAKE_CURRENT_BINARY_DIR}/third_party/opencv-install/usr"
                     NO_DEFAULT_PATH)

        # Hide this variable in cmake-gui.
        mark_as_advanced(FORCE OpenCV_DIR)
    else ()
        set(opencv_built FALSE)
    endif (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/third_party/opencv-install/usr")
# OpenCV was found from the system
else ()
    message(STATUS "Found opencv ${OpenCV_VERSION}. Assuming it includes non-free features!")
    add_custom_target(opencv)
    set(opencv_built TRUE)
endif (NOT OpenCV_FOUND)

message(STATUS "Using OpenCV from ${OpenCV_DIR}")

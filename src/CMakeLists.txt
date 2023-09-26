find_package(Threads REQUIRED)

add_library(sens_loc)
list(APPEND sens_loc_headers
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/version.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/analysis/distance.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/analysis/keypoints.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/analysis/match.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/analysis/recognition_performance.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/camera_models/pinhole.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/camera_models/equirectangular.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/camera_models/utility.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/conversion/depth_to_bearing.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/conversion/depth_to_curvature.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/conversion/depth_to_flexion.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/conversion/depth_to_laserscan.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/conversion/depth_to_max_curve.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/conversion/depth_scaling.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/conversion/util.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/io/feature.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/io/histogram.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/io/image.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/io/intrinsics.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/io/pose.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/angle_conversion.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/constants.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/coordinate.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/curvature.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/derivatives.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/eigen_types.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/image.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/pointcloud.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/rounding.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/scaling.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/math/triangles.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/plot/backprojection.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/preprocess/filter.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/util/console.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/util/correctness_util.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sens_loc/util/progress_bar_observer.h"
    )
target_sources(sens_loc PUBLIC ${sens_loc_headers})
target_sources(sens_loc PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/lib/analysis/distance.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/lib/analysis/keypoints.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/lib/analysis/match.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/lib/analysis/recognition_performance.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/lib/io/pose.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/lib/plot/backprojection.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/lib/util/console.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/lib/util/correctness_util.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/lib/util/progress_bar_observer.cpp"
    )
common_target_properties(sens_loc)

add_library(sens_loc::sens_loc ALIAS sens_loc)
target_include_directories(sens_loc
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
target_include_directories(sens_loc
    SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(sens_loc
    PUBLIC
    GSL
    rang
    Threads::Threads
    taskflow::taskflow
    Eigen3::Eigen
    ${OpenCV_LIBS}
    ${Boost_LIBRARY_DIRS}
    Iconv::Iconv
    )
target_compile_features(sens_loc INTERFACE cxx_std_17)
add_dependencies(sens_loc dependencies)

install(TARGETS sens_loc
        EXPORT sens_loc
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/apps")
if (WITH_TESTING)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/unittest")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test")
endif (WITH_TESTING)

if (WITH_BENCHMARK)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/benchmarks")
endif (WITH_BENCHMARK)

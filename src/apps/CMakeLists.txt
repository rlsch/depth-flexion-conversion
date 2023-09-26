add_library(batch_processing
    "${CMAKE_CURRENT_LIST_DIR}/util/batch_converter.h"
    "${CMAKE_CURRENT_LIST_DIR}/util/batch_converter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/util/batch_visitor.h"
    "${CMAKE_CURRENT_LIST_DIR}/util/common_structures.h"
    "${CMAKE_CURRENT_LIST_DIR}/util/colored_parse.h"
    "${CMAKE_CURRENT_LIST_DIR}/util/parallel_processing.h"
    "${CMAKE_CURRENT_LIST_DIR}/util/statistic_visitor.h"
    "${CMAKE_CURRENT_LIST_DIR}/util/tool_macro.h"
    "${CMAKE_CURRENT_LIST_DIR}/util/version_printer.h"
    "${CMAKE_CURRENT_LIST_DIR}/util/version_printer.cpp"
    )
target_link_libraries(batch_processing
    PUBLIC
    sens_loc::sens_loc
    PRIVATE
    taskflow::taskflow
    fmt::fmt)
add_library(sens_loc::batch_processing ALIAS batch_processing)
common_target_properties(batch_processing)

add_tool(depth2x "${CMAKE_CURRENT_LIST_DIR}/depth2x/main.cpp")
target_sources(depth2x
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converters.h"
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converter_scale.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converter_scale.h"
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converter_bearing.h.inl"
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converter_curvature.h.inl"
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converter_flexion.h.inl"
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converter_flexion_nxn.h.inl"
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converter_flexion_normalized.h.inl"
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converter_flexion_angle.h.inl"
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converter_laserscan.h.inl"
    "${CMAKE_CURRENT_LIST_DIR}/depth2x/converter_max_curve.h.inl"
    )
configure_file("${CMAKE_CURRENT_LIST_DIR}/kinect_intrinsic.txt"
               "${CMAKE_CURRENT_BINARY_DIR}/kinect_intrinsic.txt" COPYONLY)


add_tool(depth_filter "${CMAKE_CURRENT_LIST_DIR}/depth_filter/main.cpp")
target_sources(depth_filter
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/depth_filter/batch_filter.cpp"
    )


add_tool(feature_extractor
         "${CMAKE_CURRENT_LIST_DIR}/feature_extractor/main.cpp")
target_sources(feature_extractor
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/feature_extractor/batch_extractor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/feature_extractor/batch_extractor.h"
    )


add_tool(keypoint_plotter
         "${CMAKE_CURRENT_LIST_DIR}/keypoint_plotter/main.cpp")
target_sources(keypoint_plotter
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/keypoint_plotter/batch_plotter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/keypoint_plotter/batch_plotter.h"
    )


add_tool(feature_performance
         "${CMAKE_CURRENT_LIST_DIR}/feature_performance/main.cpp")
target_sources(feature_performance
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/icp.h"
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/icp.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/keypoint_distribution.h"
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/keypoint_distribution.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/keypoint_transform.h"
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/min_dist.h"
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/min_dist.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/matching.h"
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/matching.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/recognition_performance.h"
    "${CMAKE_CURRENT_LIST_DIR}/feature_performance/recognition_performance.cpp"
    )

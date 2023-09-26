include(test_macros)

configure_file(depth2x/kinect_intrinsic.txt
               depth2x/kinect_intrinsic.txt COPYONLY)
configure_file(depth2x/laser_intrinsic.txt
               depth2x/laser_intrinsic.txt COPYONLY)
configure_file(depth2x/data0-depth.png
               depth2x/data0-depth.png COPYONLY)
configure_file(depth2x/data1-depth.png
               depth2x/data1-depth.png COPYONLY)
configure_file(depth2x/data002-depth.png
               depth2x/data002-depth.png COPYONLY)
configure_file(depth2x/data003-depth.png
               depth2x/data003-depth.png COPYONLY)
configure_file(depth2x/data004-depth.png
               depth2x/data004-depth.png COPYONLY)
configure_file(depth2x/data005-depth.png
               depth2x/data005-depth.png COPYONLY)
configure_file(depth2x/data006-depth.png
               depth2x/data006-depth.png COPYONLY)
configure_file(depth2x/laserscan-0-depth.png
               depth2x/laserscan-0-depth.png COPYONLY)
configure_file(depth2x/laserscan-1-depth.png
               depth2x/laserscan-1-depth.png COPYONLY)
add_tool_test(depth2x test_depth2x)
add_tool_test(depth2x test_depth2x_bearing)
add_tool_test(depth2x test_depth2x_flexion)
add_tool_test(depth2x test_depth2x_gaussian_curvature)
add_tool_test(depth2x test_depth2x_mean_curvature)
add_tool_test(depth2x test_depth2x_max_curve)
add_tool_test(depth2x test_depth2x_range)
add_tool_test(depth2x test_depth2x_scale)

################################################################################

configure_file(depth2x/data0-depth.png
               depth_filter/data0-depth.png COPYONLY)
configure_file(depth2x/data1-depth.png
               depth_filter/data1-depth.png COPYONLY)
add_tool_test(depth_filter test_depth_filter)
add_tool_test(depth_filter test_depth_filter_bilateral)
add_tool_test(depth_filter test_depth_filter_median_blur)

################################################################################

configure_file(feature_extractor/flexion-0.png
               feature_extractor/flexion-0.png COPYONLY)
configure_file(feature_extractor/flexion-1.png
               feature_extractor/flexion-1.png COPYONLY)
add_tool_test(feature_extractor test_feature_extractor)
add_tool_test(feature_extractor test_feature_sift)
add_tool_test(feature_extractor test_feature_surf)
add_tool_test(feature_extractor test_feature_orb)
add_tool_test(feature_extractor test_feature_akaze)
add_tool_test(feature_extractor test_feature_agast)
add_tool_test(feature_extractor test_feature_brisk)

################################################################################

configure_file(keypoint_plotter/color-0.png
               keypoint_plotter/color-0.png COPYONLY)
configure_file(keypoint_plotter/color-1.png
               keypoint_plotter/color-1.png COPYONLY)
configure_file(keypoint_plotter/depth-0.png
               keypoint_plotter/depth-0.png COPYONLY)
configure_file(keypoint_plotter/depth-1.png
               keypoint_plotter/depth-1.png COPYONLY)
configure_file(keypoint_plotter/flexion-0.png
               keypoint_plotter/flexion-0.png COPYONLY)
configure_file(keypoint_plotter/flexion-1.png
               keypoint_plotter/flexion-1.png COPYONLY)
configure_file(keypoint_plotter/sift-0.feat
               keypoint_plotter/sift-0.feat COPYONLY)
configure_file(keypoint_plotter/sift-1.feat
               keypoint_plotter/sift-1.feat COPYONLY)
configure_file(keypoint_plotter/compressed-0.feat.gz
               keypoint_plotter/compressed-0.feat.gz COPYONLY)
configure_file(keypoint_plotter/compressed-1.feat.gz
               keypoint_plotter/compressed-1.feat.gz COPYONLY)
add_tool_test(keypoint_plotter test_keypoint_plotter)
add_tool_test(keypoint_plotter test_keypoint_plotter_only_feature_files)
add_tool_test(keypoint_plotter test_keypoint_plotter_overwrite_image)

################################################################################

configure_file(feature_performance/sift-0.feat
               feature_performance/sift-0.feat COPYONLY)
configure_file(feature_performance/sift-1.feat
               feature_performance/sift-1.feat COPYONLY)
configure_file(feature_performance/orb-0.feat
               feature_performance/orb-0.feat COPYONLY)
configure_file(feature_performance/orb-1.feat
               feature_performance/orb-1.feat COPYONLY)
configure_file(feature_performance/surf-1-octave-0.feat.gz
               feature_performance/surf-1-octave-0.feat.gz COPYONLY)
configure_file(feature_performance/surf-1-octave-1.feat.gz
               feature_performance/surf-1-octave-1.feat.gz COPYONLY)
configure_file(feature_performance/surf-1-octave-0.feat.gz
               feature_performance/surf-1-octave-0.feat.gz COPYONLY)
configure_file(feature_performance/does-not-exist-1.feat.gz
               feature_performance/does-not-exist-1.feat.gz COPYONLY)
configure_file(feature_performance/does-not-exist-0.feat.gz
               feature_performance/does-not-exist-0.feat.gz COPYONLY)
configure_file(feature_performance/does-not-exist-1.feat.gz
               feature_performance/does-not-exist-1.feat.gz COPYONLY)
configure_file(feature_performance/does-not-exist-0.feat.gz
               feature_performance/does-not-exist-0.feat.gz COPYONLY)
configure_file(feature_performance/does-not-exist-1.feat.gz
               feature_performance/does-not-exist-1.feat.gz COPYONLY)
configure_file(feature_performance/kinect_intrinsic.txt
               feature_performance/kinect_intrinsic.txt COPYONLY)
configure_file(feature_performance/filtered-0.png
               feature_performance/filtered-0.png COPYONLY)
configure_file(feature_performance/filtered-1.png
               feature_performance/filtered-1.png COPYONLY)
configure_file(feature_performance/flexion-1.png
               feature_performance/flexion-1.png COPYONLY)
configure_file(feature_performance/pose-0.pose
               feature_performance/pose-0.pose COPYONLY)
configure_file(feature_performance/pose-1.pose
               feature_performance/pose-1.pose COPYONLY)
configure_file(feature_performance/distortion_mask.png
               feature_performance/distortion_mask.png COPYONLY)
configure_file(feature_performance/distortion_mask_bad.png
               feature_performance/distortion_mask_bad.png COPYONLY)
add_tool_test(feature_performance test_feature_performance)
add_tool_test(feature_performance test_feature_performance_min_distance)
add_tool_test(feature_performance test_feature_performance_keypoints)
add_tool_test(feature_performance test_feature_performance_matching)
add_tool_test(feature_performance test_feature_performance_recognition_performance)

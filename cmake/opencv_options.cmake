message(STATUS "Disabling OpenCV Tests due to complexity")
set(WITH_DEP_TESTING OFF)

message(STATUS "Build-Tools used for opencv build")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Linker: ${CMAKE_LINKER}")
message(STATUS "Contrib Modules: ${OPENCV_CONTRIB_MODULES}")

list(APPEND opencv_options
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_LINKER=${CMAKE_LINKER}
    "$<$<BOOL:${USE_LIBCXX}>:-DCMAKE_CXX_FLAGS=-stdlib=libc++>"
    "$<$<BOOL:${USE_LIBCXX}>:-DCMAKE_EXE_LINKER_FLAGS=-lc++abi\ -stdlib=libc++>"
    "$<$<BOOL:${WITH_STATIC_STDCXXLIB}>:-DCMAKE_EXE_LINKER_FLAGS=-static-libstdc++\ -static-libgcc>"
    -DBUILD_CUDA_STUBS:BOOL=OFF
    -DBUILD_DOCS:BOOL=OFF
    -DBUILD_EXAMPLES:BOOL=OFF
    -DBUILD_IPP_IW:BOOL=OFF
    -DBUILD_ITT:BOOL=OFF
    -DBUILD_JASPER:BOOL=OFF
    -DBUILD_JAVA:BOOL=OFF
    -DBUILD_JPEG:BOOL=OFF
    -DBUILD_LIST:STRING=
    -DBUILD_OPENEXR:BOOL=OFF
    -DBUILD_PACKAGE:BOOL=OFF
    -DBUILD_PERF_TESTS:BOOL=OFF
    -DBUILD_PNG:BOOL=ON
    -DBUILD_PROTOBUF:BOOL=OFF
    -DBUILD_SHARED_LIBS:BOOL=OFF
    -DBUILD_TBB:BOOL=OFF
    -DBUILD_TESTS:BOOL=OFF
    -DBUILD_TIFF:BOOL=OFF
    -DBUILD_USE_SYMLINKS:BOOL=OFF
    -DBUILD_WEBP:BOOL=OFF
    -DBUILD_WITH_DEBUG_INFO:BOOL=OFF
    -DBUILD_WITH_DYNAMIC_IPP:BOOL=OFF
    -DBUILD_ZLIB:BOOL=ON
    -DBUILD_opencv_apps:BOOL=OFF
    -DBUILD_opencv_aruco:BOOL=OFF
    -DBUILD_opencv_bgsegm:BOOL=OFF
    -DBUILD_opencv_bioinspired:BOOL=OFF
    -DBUILD_opencv_calib3d:BOOL=ON
    -DBUILD_opencv_ccalib:BOOL=OFF
    -DBUILD_opencv_core:BOOL=ON
    -DBUILD_opencv_datasets:BOOL=OFF
    -DBUILD_opencv_dnn:BOOL=OFF
    -DBUILD_opencv_dnn_objdetect:BOOL=OFF
    -DBUILD_opencv_dnn_superres:BOOL=OFF
    -DBUILD_opencv_dpm:BOOL=OFF
    -DBUILD_opencv_face:BOOL=OFF
    -DBUILD_opencv_features2d:BOOL=ON
    -DBUILD_opencv_flann:BOOL=ON
    -DBUILD_opencv_freetype:BOOL=OFF
    -DBUILD_opencv_fuzzy:BOOL=OFF
    -DBUILD_opencv_gapi:BOOL=OFF
    -DBUILD_opencv_hfs:BOOL=OFF
    -DBUILD_opencv_highgui:BOOL=OFF
    -DBUILD_opencv_img_hash:BOOL=OFF
    -DBUILD_opencv_imgcodecs:BOOL=ON
    -DBUILD_opencv_imgproc:BOOL=ON
    -DBUILD_opencv_java_bindings_generator:BOOL=OFF
    -DBUILD_opencv_js:BOOL=OFF
    -DBUILD_opencv_line_descriptor:BOOL=OFF
    -DBUILD_opencv_ml:BOOL=OFF
    -DBUILD_opencv_objdetect:BOOL=OFF
    -DBUILD_opencv_optflow:BOOL=OFF
    -DBUILD_opencv_phase_unwrapping:BOOL=OFF
    -DBUILD_opencv_photo:BOOL=OFF
    -DBUILD_opencv_plot:BOOL=OFF
    -DBUILD_opencv_python3:BOOL=OFF
    -DBUILD_opencv_python_bindings_generator:BOOL=OFF
    -DBUILD_opencv_python_tests:BOOL=OFF
    -DBUILD_opencv_quality:BOOL=OFF
    -DBUILD_opencv_reg:BOOL=OFF
    -DBUILD_opencv_rgbd:BOOL=ON
    -DBUILD_opencv_saliency:BOOL=OFF
    -DBUILD_opencv_sfm:BOOL=OFF
    -DBUILD_opencv_shape:BOOL=OFF
    -DBUILD_opencv_stereo:BOOL=OFF
    -DBUILD_opencv_stitching:BOOL=OFF
    -DBUILD_opencv_structured_light:BOOL=OFF
    -DBUILD_opencv_superres:BOOL=OFF
    -DBUILD_opencv_surface_matching:BOOL=OFF
    -DBUILD_opencv_text:BOOL=OFF
    -DBUILD_opencv_tracking:BOOL=OFF
    -DBUILD_opencv_ts:BOOL=OFF
    -DBUILD_opencv_video:BOOL=ON
    -DBUILD_opencv_videoio:BOOL=OFF
    -DBUILD_opencv_videostab:BOOL=OFF
    -DBUILD_opencv_world:BOOL=OFF
    -DBUILD_opencv_xfeatures2d:BOOL=ON
    -DBUILD_opencv_ximgproc:BOOL=ON
    -DBUILD_opencv_xobjdetect:BOOL=OFF
    -DBUILD_opencv_xphoto:BOOL=OFF
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCPU_BASELINE:STRING=SSE4_2
    -DCPU_DISPATCH:STRING=SSE4_2,AVX,AVX2
    -DCV_DISABLE_OPTIMIZATION:BOOL=OFF
    -DCV_ENABLE_INTRINSICS:BOOL=ON
    -DCV_TRACE:BOOL=OFF
    -DENABLE_BUILD_HARDENING:BOOL=OFF
    -DENABLE_CCACHE:BOOL=OFF
    -DENABLE_CONFIG_VERIFICATION:BOOL=ON
    -DENABLE_COVERAGE:BOOL=OFF
    -DENABLE_FAST_MATH:BOOL=$<BOOL:${WITH_FAST_MATH}>
    -DENABLE_FLAKE8:BOOL=OFF
    -DENABLE_GNU_STL_DEBUG:BOOL=OFF
    -DENABLE_IMPL_COLLECTION:BOOL=OFF
    -DENABLE_INSTRUMENTATION:BOOL=OFF
    -DENABLE_LTO:BOOL=$<BOOL:${WITH_IPO}>
    -DENABLE_NOISY_WARNINGS:BOOL=OFF
    -DENABLE_OMIT_FRAME_POINTER:BOOL=ON
    -DENABLE_PIC:BOOL=ON
    -DENABLE_PRECOMPILED_HEADERS:BOOL=OFF
    -DENABLE_PROFILING:BOOL=OFF
    -DENABLE_PYLINT:BOOL=OFF
    -DENABLE_SOLUTION_FOLDERS:BOOL=OFF
    -DGENERATE_ABI_DESCRIPTOR:BOOL=OFF
    -DINSTALL_CREATE_DISTRIB:BOOL=OFF
    -DINSTALL_C_EXAMPLES:BOOL=OFF
    -DINSTALL_PYTHON_EXAMPLES:BOOL=OFF
    -DINSTALL_TESTS:BOOL=OFF
    -DINSTALL_TO_MANGLED_PATHS:BOOL=OFF
    -DOPENCV_DNN_CUDA:BOOL=OFF
    -DOPENCV_DNN_OPENCL:BOOL=OFF
    -DOPENCV_DOWNLOAD_PATH:PATH=${OPENCV_SOURCE}/.cache
    -DOPENCV_DUMP_HOOKS_FLOW:BOOL=OFF
    -DOPENCV_ENABLE_MEMORY_SANITIZER:BOOL=$<BOOL:${WITH_MSAN}>
    -DOPENCV_ENABLE_NONFREE:BOOL=ON
    -DOPENCV_ENABLE_MEMALIGN:BOOL=OFF
    -DOPENCV_EXTRA_MODULES_PATH:PATH=${OPENCV_CONTRIB_MODULES}
    -DOPENCV_FORCE_3RDPARTY_BUILD:BOOL=OFF
    -DOPENCV_FORCE_PYTHON_LIBS:BOOL=OFF
    -DOPENCV_GENERATE_PKGCONFIG:BOOL=OFF
    -DOPENCV_GENERATE_SETUPVARS:BOOL=OFF
    -DOPENCV_PYTHON3_VERSION:BOOL=OFF
    -DOPENCV_WARNINGS_ARE_ERRORS:BOOL=OFF
    -DPROTOBUF_UPDATE_FILES:BOOL=OFF
    -DUSE_WIN32_FILEIO:BOOL=OFF
    -DWITH_1394:BOOL=OFF
    -DWITH_ADE:BOOL=OFF
    -DWITH_ARAVIS:BOOL=OFF
    -DWITH_ARITH_DEC:BOOL=OFF
    -DWITH_ARITH_ENC:BOOL=OFF
    -DWITH_CLP:BOOL=OFF
    -DWITH_CUDA:BOOL=OFF
    -DWITH_DIRECTX:BOOL=OFF
    -DWITH_EIGEN:BOOL=ON
    -DWITH_FFMPEG:BOOL=OFF
    -DWITH_GDAL:BOOL=OFF
    -DWITH_GDCM:BOOL=OFF
    -DWITH_GPHOTO2:BOOL=OFF
    -DWITH_GSTREAMER:BOOL=OFF
    -DWITH_GTK:BOOL=OFF
    -DWITH_GTK_2_X:BOOL=OFF
    -DWITH_HALIDE:BOOL=OFF
    -DWITH_HPX:BOOL=OFF
    -DWITH_IMGCODEC_HDR:BOOL=OFF
    -DWITH_IMGCODEC_PFM:BOOL=OFF
    -DWITH_IMGCODEC_PXM:BOOL=OFF
    -DWITH_IMGCODEC_SUNRASTER:BOOL=OFF
    -DWITH_INF_ENGINE:BOOL=OFF
    -DWITH_IPP:BOOL=OFF
    -DWITH_ITT:BOOL=OFF
    -DWITH_JASPER:BOOL=OFF
    -DWITH_JPEG:BOOL=OFF
    -DWITH_LAPACK:BOOL=OFF
    -DWITH_LIBREALSENSE:BOOL=OFF
    -DWITH_MFX:BOOL=OFF
    -DWITH_OPENCL:BOOL=ON
    -DWITH_OPENCLAMDBLAS:BOOL=OFF
    -DWITH_OPENCLAMDFFT:BOOL=OFF
    -DWITH_OPENCL_D3D11_NV:BOOL=OFF
    -DWITH_OPENCL_SVM:BOOL=OFF
    -DWITH_OPENEXR:BOOL=OFF
    -DWITH_OPENGL:BOOL=OFF
    -DWITH_OPENMP:BOOL=OFF
    -DWITH_OPENNI:BOOL=OFF
    -DWITH_OPENNI2:BOOL=OFF
    -DWITH_OPENVX:BOOL=OFF
    -DWITH_PNG:BOOL=ON
    -DWITH_PROTOBUF:BOOL=OFF
    -DWITH_PTHREADS_PF:BOOL=OFF
    -DWITH_PVAPI:BOOL=OFF
    -DWITH_QT:BOOL=OFF
    -DWITH_QUIRC:BOOL=OFF
    -DWITH_TBB:BOOL=OFF
    -DWITH_TIFF:BOOL=OFF
    -DWITH_V4L:BOOL=OFF
    -DWITH_VA:BOOL=OFF
    -DWITH_VA_INTEL:BOOL=OFF
    -DWITH_VTK:BOOL=OFF
    -DWITH_VULKAN:BOOL=OFF
    -DWITH_WEBP:BOOL=OFF
    -DWITH_XIMEA:BOOL=OFF
    -DWITH_XINE:BOOL=OFF
    -Dccitt:BOOL=OFF
    -Dlogluv:BOOL=OFF
    -Dlzw:BOOL=OFF
    -Dmdi:BOOL=OFF
    -Dnext:BOOL=OFF
    -Dold-jpeg:BOOL=OFF
    -Dopencv_dnn_PERF_CAFFE:BOOL=OFF
    -Dopencv_dnn_PERF_CLCAFFE:BOOL=OFF
    -Dpackbits:BOOL=ON
    -Dthunder:BOOL=ON
)

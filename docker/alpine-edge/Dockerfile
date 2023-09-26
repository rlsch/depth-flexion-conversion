FROM alpine:edge
LABEL maintainer="jonas.toth@student.tu-freiberg.de"

RUN apk update \
 && apk add bash \
            build-base \
            ninja \
            cmake extra-cmake-modules \
            gcc g++ binutils \
            linux-headers \
            wget \
            unzip \
            rsync \
            boost boost-dev \
            eigen

ARG cv_version="4.3.0"
RUN mkdir -p /opencv/build_cv && cd /opencv \
 && wget --quiet https://github.com/opencv/opencv/archive/$cv_version.zip \
 && unzip $cv_version.zip && rm $cv_version.zip \
 && wget --quiet https://github.com/opencv/opencv_contrib/archive/$cv_version.zip \
 && unzip $cv_version.zip && rm $cv_version.zip \
 && cd /opencv/build_cv \
 && CC=gcc CXX=g++ cmake /opencv/opencv-$cv_version/ \
    -G Ninja \
    -DCMAKE_EXE_LINKER_FLAGS="-static-libstdc++ -static-libgcc" \
    -DCMAKE_BUILD_TYPE=Release \
    -DOPENCV_EXTRA_MODULES_PATH="/opencv/opencv_contrib-$cv_version/modules/" \
    -DBUILD_opencv_java_bindings_generator=OFF \
    -DBUILD_opencv_js=OFF \
    -DBUILD_opencv_python_bindings_generator=OFF \
    -DBUILD_opencv_python3=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DENABLE_BUILD_HARDENING=ON \
    -DENABLE_PRECOMPILED_HEADERS=OFF \
    -DCPU_BASELINE:STRING="SSE3" \
    -DCPU_DISPATCH:STRING="SSE4_1;SSE4_2;AVX;FP16;AVX2" \
    -DBUILD_TESTS=OFF \
    -DINSTALL_TESTS=OFF \
    -DENABLE_LTO=OFF \
    -DOPENCV_PYTHON3_VERSION=OFF \
    -DOPENCV_ENABLE_NONFREE=ON \
 && ninja \
 && ninja install \
 && cd / && rm -rf /opencv

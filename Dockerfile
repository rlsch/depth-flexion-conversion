FROM ubuntu:22.04 AS toolchain
LABEL maintainer="jonas.toth@student.tu-freiberg.de"

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update \
 && apt-get dist-upgrade -y \
 && apt-get install -y \
    build-essential \
    cmake \
    g++ gcc \
    git \
    ninja-build \
    wget \
    unzip \
    python3-pip \
    libeigen3-dev \
    libboost-all-dev \
    libpng-dev \
    libjpeg-dev

ARG cv_version="4.3.0"
RUN mkdir -p /opencv/build_cv && cd /opencv \
 && wget --quiet https://github.com/opencv/opencv/archive/$cv_version.zip \
 && unzip $cv_version.zip && mv opencv-$cv_version opencv && rm $cv_version.zip \
 && wget --quiet https://github.com/opencv/opencv_contrib/archive/$cv_version.zip \
 && unzip $cv_version.zip && mv opencv_contrib-$cv_version opencv_contrib && rm $cv_version.zip \
 && cd /opencv/build_cv \
 && CC=gcc CXX=g++ cmake /opencv/opencv \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DOPENCV_EXTRA_MODULES_PATH="/opencv/opencv_contrib/modules/" \
    -DBUILD_opencv_java_bindings_generator=OFF \
    -DBUILD_opencv_js=OFF \
    -DBUILD_opencv_python_bindings_generator=OFF \
    -DBUILD_opencv_python3=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DENABLE_PRECOMPILED_HEADERS=OFF \
    -DBUILD_TESTS=OFF \
    -DINSTALL_TESTS=OFF \
    -DENABLE_LTO=OFF \
    -DOPENCV_PYTHON3_VERSION=OFF \
    -DOPENCV_ENABLE_NONFREE=ON \
 && ninja \
 && ninja install \
 && cd /opencv && rm -rf build_cv

FROM toolchain AS project
WORKDIR /project
COPY . /project
ENV WITH_TESTING=ON
RUN ./scripts/ci/normal_build.bash

FROM ubuntu:22.04 AS final
RUN apt-get update \
 && apt-get install -y \
    libpng16-16 \
    libjpeg-turbo8 \
 && apt-get clean
COPY --from=project /project/build/src/apps/depth2x /usr/bin/depth2x
COPY --from=project /project/build/src/apps/depth_filter /usr/bin/depth_filter

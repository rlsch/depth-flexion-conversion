FROM ubuntu:20.04
LABEL maintainer="jonas.toth@student.tu-freiberg.de"

ENV DEBIAN_FRONTEND noninteractive
RUN apt update \
 && apt dist-upgrade -y \
 && apt install -y \
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

ARG cv_version="4.5.0"
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

ARG boost_version="1.73.0"
ARG boost_version_string="1_73_0"
RUN mkdir -p /boost cd /boost \
 && wget --quiet https://boostorg.jfrog.io/artifactory/main/release/${boost_version}/source/boost_${boost_version_string}.tar.gz  \
 && tar xf boost_$boost_version_string.tar.gz && cd boost_${boost_version_string} \
 && ./bootstrap.sh --prefix=/usr \
 && ./b2 install link=static variant=release --without-python \
 && cd / && rm -rf /boost

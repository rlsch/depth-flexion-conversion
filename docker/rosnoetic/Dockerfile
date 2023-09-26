FROM ros:noetic
LABEL maintainer="robert.loesch@informatik.tu-freiberg.de"

RUN apt-get update \
 && apt-get install -y \
    git \
    python3-pip \
    ninja-build \
    libeigen3-dev \
    libboost-all-dev \
    software-properties-common \
    wget \
    rsync \
    libopenjp2-7-dev \
 && add-apt-repository ppa:ubuntu-toolchain-r/test \
 && apt-get update \
 && apt-get install -y  \
    gcc-9 g++-9 \
    unzip wget \
 && pip3 install --upgrade pip 
# && pip3 install cmake

RUN apt install -y \
    ros-noetic-image-transport \
    ros-noetic-cv-bridge

ARG cmake_version="3.26.4"
RUN apt-get update \
 && mkdir /cmake && cd /cmake \
 && wget https://github.com/Kitware/CMake/releases/download/v$cmake_version/cmake-$cmake_version-Linux-x86_64.sh \
 && chmod +x cmake-*.sh \
 && ./cmake-$cmake_version-Linux-x86_64.sh --prefix=/usr --skip-license \
 && rm -rf * && cd /

ARG cv_version="4.5.0"
RUN mkdir -p /opencv/build_cv && cd /opencv \
 && wget --quiet https://github.com/opencv/opencv/archive/$cv_version.zip \
 && unzip $cv_version.zip && rm $cv_version.zip \
 && wget --quiet https://github.com/opencv/opencv_contrib/archive/$cv_version.zip \
 && unzip $cv_version.zip && rm $cv_version.zip \
 && cd /opencv/build_cv \
 && CC=gcc CXX=g++ cmake /opencv/opencv-$cv_version/ \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DOPENCV_EXTRA_MODULES_PATH="/opencv/opencv_contrib-$cv_version/modules/" \
    -DBUILD_opencv_java_bindings_generator=OFF \
    -DBUILD_opencv_js=OFF \
    -DBUILD_opencv_python_bindings_generator=OFF \
    -DBUILD_opencv_python3=OFF \
    -DBUILD_SHARED_LIBS=OFF \
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

ARG boost_version="1.73.0"
ARG boost_version_string="1_73_0"
RUN mkdir -p /boost cd /boost \
 && wget --quiet https://boostorg.jfrog.io/artifactory/main/release/${boost_version}/source/boost_${boost_version_string}.tar.gz  \
 && tar xf boost_$boost_version_string.tar.gz && cd boost_${boost_version_string} \
 && ./bootstrap.sh --prefix=/usr \
 && ./b2 install link=static variant=release --without-python \
 && cd / && rm -rf /boost

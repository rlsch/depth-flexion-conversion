FROM ubuntu:14.04
LABEL maintainer="jonas.toth@student.tu-freiberg.de"

ARG cmake_version="3.18.0"
RUN apt-get update \
 && apt-get install -y wget python-software-properties rsync unzip build-essential software-properties-common \
 && wget --quiet https://github.com/Kitware/CMake/releases/download/v$cmake_version/cmake-$cmake_version-Linux-x86_64.sh \
 && sh cmake-$cmake_version-Linux-x86_64.sh --prefix=/usr/local --skip-license \
 && rm cmake-$cmake_version* \
 && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
 && apt-get update \
 && apt-get install -y g++-9 gcc-9 make

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 10 \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 10 \
 && update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 20 \
 && update-alternatives --set cc /usr/bin/gcc \
 && update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 20 \
 && update-alternatives --set c++ /usr/bin/g++

ARG ninja_version="1.9.0"
WORKDIR /ninja
RUN wget --quiet https://github.com/ninja-build/ninja/archive/v$ninja_version.tar.gz \
 && tar xf v$ninja_version.tar.gz && cd ninja-$ninja_version \
 && ./configure.py --bootstrap \
 && ./ninja \
 && mv ninja /usr/bin/ \
 && cd / \
 && rm -rf /ninja

ARG cv_version="4.3.0"
RUN mkdir -p /opencv/build_cv && cd /opencv \
 && wget --quiet https://github.com/opencv/opencv/archive/$cv_version.zip \
 && unzip $cv_version.zip && rm $cv_version.zip \
 && wget --quiet https://github.com/opencv/opencv_contrib/archive/$cv_version.zip \
 && unzip $cv_version.zip && rm $cv_version.zip \
 && cd /opencv/build_cv \
 && CC=gcc-9 CXX=g++-9 cmake /opencv/opencv-$cv_version/ \
    -G Ninja \
    -DCMAKE_EXE_LINKER_FLAGS="-static-libstdc++ -static-libgcc" \
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

ARG eigen_version="3.3.7"
RUN mkdir -p /eigen3 && cd /eigen3 \
# && wget --quiet http://bitbucket.org/eigen/eigen/get/$eigen_version.tar.gz \
 && wget --quiet https://gitlab.com/libeigen/eigen/-/archive/$eigen_version/eigen-$eigen_version.tar.gz \
 && tar xf eigen-$eigen_version.tar.gz && mv eigen-$eigen_version eigen && cd eigen \
 && mkdir build && cd build \
 && CC=gcc-9 CXX=g++-9 cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=/usr \
 && ninja install

ARG boost_version="1.73.0"
ARG boost_version_string="1_73_0"
RUN mkdir -p /boost cd /boost \
 && wget --quiet https://boostorg.jfrog.io/artifactory/main/release/${boost_version}/source/boost_${boost_version_string}.tar.gz  \
 && tar xf boost_$boost_version_string.tar.gz && cd boost_${boost_version_string} \
 && ./bootstrap.sh --prefix=/usr \
 && ./b2 install link=static variant=release --without-python \
 && cd / && rm -rf /boost

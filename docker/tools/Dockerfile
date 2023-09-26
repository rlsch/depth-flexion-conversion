FROM ubuntu:18.04
LABEL maintainer="jonas.toth@student.tu-freiberg.de"

RUN apt-get update \
 && apt-get install -y wget gnupg2 \
 && wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
 && echo "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-9 main" >> /etc/apt/sources.list \
 && echo "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-10 main" >> /etc/apt/sources.list \
 && apt-get update \
 && apt-get dist-upgrade -y \
 && apt-get install -y \
    build-essential \
    git \
    ninja-build extra-cmake-modules \
    software-properties-common \
    unzip \
    python3-pip \
    libpng-dev \
    libjpeg-dev \
    rsync \
    valgrind \
    unzip \
 && add-apt-repository ppa:ubuntu-toolchain-r/test \
 && apt-get update \
 && apt-get install -y  \
    clang-8 lld-8 \
    clang-9 lld-9 \
    clang-10 clang-tools-10 lld-10 clang-tidy-10 \
    libc++1-10 libc++-10-dev libc++abi1-10 libc++abi-10-dev \
    gcc-7 g++-7 \
    gcc-8 g++-8 \
    gcc-9 g++-9 \
    lcov gcovr \
 && pip3 install --upgrade pip \
 && pip install cmake==3.22.0

ARG cv_version="4.3.0"
RUN mkdir -p /opencv/build_cv && cd /opencv \
 && wget --quiet https://github.com/opencv/opencv/archive/$cv_version.zip \
 && unzip $cv_version.zip && mv opencv-$cv_version opencv && rm $cv_version.zip \
 && wget --quiet https://github.com/opencv/opencv_contrib/archive/$cv_version.zip \
 && unzip $cv_version.zip && mv opencv_contrib-$cv_version opencv_contrib && rm $cv_version.zip \
 && cd /opencv/build_cv \
 && CC=gcc-9 CXX=g++-9 cmake /opencv/opencv/ \
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

ARG eigen_version="3.3.7"
RUN mkdir -p /eigen3 && cd /eigen3 \
 && wget --quiet https://gitlab.com/libeigen/eigen/-/archive/$eigen_version/eigen-$eigen_version.tar.gz \
 && tar xf eigen-$eigen_version.tar.gz && mv eigen-$eigen_version eigen && cd eigen \
 && mkdir build && cd build \
 && cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=/usr \
 && ninja install

ARG boost_version="1.73.0"
ARG boost_version_string="1_73_0"
RUN mkdir -p /boost cd /boost \
 && wget --quiet https://boostorg.jfrog.io/artifactory/main/release/${boost_version}/source/boost_${boost_version_string}.tar.gz  \
 && tar xf boost_$boost_version_string.tar.gz && rm -f boost_$boost_version_string.tar.gz && cd boost_$boost_version_string \
 && ./bootstrap.sh --prefix=/usr \
 && ./b2 install link=static variant=release --without-python \
 && cd / && rm -rf /boost

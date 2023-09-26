FROM ubuntu:20.04
LABEL maintainer="robert.loesch@informatik.tu-freiberg.de"

ARG DEBIAN_FRONTEND=noninteractive
ARG cmake_version="3.26.4"
RUN apt-get update \
 && apt-get install -y wget gnupg2 \
 && wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
 && echo "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-10 main" >> /etc/apt/sources.list \
 && echo "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-16 main" >> /etc/apt/sources.list \
 && apt-get update \
 && apt-get dist-upgrade -y \
 && apt-get install -y \
    build-essential \
    git \
    ninja-build \
    libeigen3-dev \
    libboost-all-dev \
    clang-10 \
    clang-16 \
    unzip \
    rsync \
 && mkdir /cmake && cd /cmake \
 && wget https://github.com/Kitware/CMake/releases/download/v$cmake_version/cmake-$cmake_version-Linux-x86_64.sh \
 && chmod +x cmake-*.sh \
 && ./cmake-$cmake_version-Linux-x86_64.sh --prefix=/usr --skip-license \
 && rm -rf * && cd /

WORKDIR /llvm
RUN git clone --depth=1 https://github.com/llvm/llvm-project.git --branch="release/10.x"

WORKDIR /llvm/build
RUN cmake ../llvm-project/llvm \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DLLVM_USE_SANITIZER=Memory \
        -DLLVM_ENABLE_PROJECTS="libcxx;libcxxabi" \
        -DLLVM_TARGETS_TO_BUILD="X86" \
        -DLLVM_TABLEGEN=/usr/bin/llvm-tblgen-10 \
        -DCMAKE_C_COMPILER=clang-10 \
        -DCMAKE_CXX_COMPILER=clang++-10
RUN ninja cxx \
 && ninja install-cxx install-cxxabi \
 && cd /llvm \
 && rm -rf build \
 && rm -rf llvm-project
ENV LD_LIBRARY_PATH="/usr/local/lib/"

WORKDIR /opencv
ARG cv_version="4.5.0"
RUN mkdir -p build_cv \
 && wget --quiet https://github.com/opencv/opencv/archive/$cv_version.zip \
 && unzip $cv_version.zip && mv opencv-$cv_version opencv && rm $cv_version.zip \
 && wget --quiet https://github.com/opencv/opencv_contrib/archive/$cv_version.zip \
 && unzip $cv_version.zip && mv opencv_contrib-$cv_version opencv_contrib && rm $cv_version.zip \
 && cd /opencv/build_cv \
 && CC=clang-10 CXX=clang++-10 \
    CFLAGS="-fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer" \
    CXXFLAGS="-stdlib=libc++ -fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer" \
    LDFLAGS="-lc++abi -stdlib=libc++" \
    cmake /opencv/opencv/ \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DOPENCV_EXTRA_MODULES_PATH="/opencv/opencv_contrib/modules/" \
    -DBUILD_PNG=ON \
    -DBUILD_ZLIB=ON \
    -DBUILD_IPP_IW=OFF \
    -DBUILD_ITT=OFF \
    -DBUILD_JASPER=OFF \
    -DBUILD_JAVA=OFF \
    -DBUILD_JPEG=OFF \
    -DBUILD_PROTOBUF=OFF \
    -DBUILD_TIFF=OFF \
    -DBUILD_opencv_java_bindings_generator=OFF \
    -DBUILD_opencv_js=OFF \
    -DBUILD_opencv_python_bindings_generator=OFF \
    -DBUILD_opencv_python3=OFF \
    -DBUILD_opencv_ml=OFF \
    -DBUILD_opencv_optflow=OFF \
    -DBUILD_opencv_dnn=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DOPENCV_ENABLE_MEMORY_SANITIZER=ON \
    -DENABLE_PRECOMPILED_HEADERS=OFF \
    -DBUILD_TESTS=OFF \
    -DINSTALL_TESTS=OFF \
    -DENABLE_LTO=OFF \
    -DOPENCV_PYTHON3_VERSION=OFF \
    -DOPENCV_ENABLE_NONFREE=ON \
    -DWITH_EIGEN=ON \
    -DWITH_JPEG=OFF \
    -DWITH_TIFF=OFF \
    -DWITH_ITT=OFF \
    -DWITH_IPP=OFF \
    -DWITH_ADE=OFF \
 && ninja \
 && ninja install \
 && cd /opencv && rm -rf build_cv

RUN update-alternatives --install /usr/bin/clang clang /usr/lib/llvm-10/bin/clang 10 \
 && update-alternatives --install /usr/bin/clang++ clang++ /usr/lib/llvm-10/bin/clang++ 10 \
 && update-alternatives --install /usr/bin/cc cc /usr/bin/clang 20 \
 && update-alternatives --set cc /usr/bin/clang \
 && update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 20 \
 && update-alternatives --set c++ /usr/bin/clang++

ARG boost_version="1.73.0"
ARG boost_version_string="1_73_0"
RUN mkdir -p /boost && cd /boost \
 && wget --quiet https://boostorg.jfrog.io/artifactory/main/release/${boost_version}/source/boost_${boost_version_string}.tar.gz  \
 && tar xf boost_$boost_version_string.tar.gz && cd boost_${boost_version_string} \
 && CC=clang-10 CXX=clang++-10 \
    ./bootstrap.sh --prefix=/usr --with-toolset=clang \
 && ./b2 install link=static variant=release \
    cxxflags="-stdlib=libc++ -fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer" \
    linkflags="-lc++abi -stdlib=libc++" \
    --without-python \
 && cd / && rm -rf /boost

ARG fmt_version="6.1.2"
RUN mkdir -p /fmtlib && cd /fmtlib \
 && wget --quiet https://github.com/fmtlib/fmt/archive/${fmt_version}.tar.gz \
 && tar xf $fmt_version.tar.gz && mv fmt-${fmt_version} fmt \
 && mkdir build && cd build \
 && cd /fmtlib/build \
 && CC=clang-10 CXX=clang++-10 \
    CFLAGS="-fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer" \
    CXXFLAGS="-stdlib=libc++ -fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer" \
    LDFLAGS="-lc++abi -stdlib=libc++" \
    cmake /fmtlib/fmt/ -G Ninja -DCMAKE_INSTALL_PREFIX=/usr \
 && ninja install

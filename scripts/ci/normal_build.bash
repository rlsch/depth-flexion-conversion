#!/bin/bash

# This script executes a normal build and is configurable via environment
# variables. These are set in '.gitlab-ci.yml'.

set -e
set -o pipefail

SCRIPT_FILE=$(readlink -f "$0")
SCRIPT_PATH=$(dirname "${SCRIPT_FILE}")

# shellcheck source=../log_helpers.sh
. "${SCRIPT_PATH}/../log_helpers.sh"

if [[ -z "$BUILD_TYPE" ]]; then
    BUILD_TYPE="RelWithDebInfo"
fi
if [[ -z "$FORCE_BUNDLED_CV" ]]; then
    FORCE_BUNDLED_CV=OFF
fi
if [[ -z "$FORCE_BUNDLED_EIGEN" ]]; then
    FORCE_BUNDLED_EIGEN=OFF
fi
if [[ -z "$WITH_TESTING" ]]; then
    WITH_TESTING=ON
fi
if [[ -z "$WITH_BENCHMARK" ]]; then
    WITH_BENCHMARK=OFF
fi
if [[ -z "$WITH_CONTRACT_EXCEPTION" ]]; then
    WITH_CONTRACT_EXCEPTION=ON
fi
if [[ -z "$WITH_ASAN" ]]; then
    WITH_ASAN=OFF
fi
if [[ -z "$WITH_UBSAN" ]]; then
    WITH_UBSAN=OFF
fi
if [[ -z "$WITH_TSAN" ]]; then
    WITH_TSAN=OFF
fi
if [[ -z "$WITH_IPO" ]]; then
    WITH_IPO=OFF
fi
if [[ -z "$WITH_STATIC_STDCXXLIB" ]]; then
    WITH_STATIC_STDCXXLIB=OFF
fi
if [[ -z "$CC" ]]; then
    export CC=gcc
fi
if [[ -z "$CXX" ]]; then
    export CXX=g++
fi
if [[ -z "$LD" ]]; then
    LD=ld
fi
if [[ -z "$USE_LIBCXX" ]]; then
    USE_LIBCXX=OFF
fi
if [[ -z "$WITH_AVX2" ]]; then
    WITH_AVX2=ON
fi
if [[ -z "$WITH_SSE42" ]]; then
    WITH_SSE42=OFF
fi

echo "Start working"

print_info "Running build with the following environment"
env

print_info "Creating build directory"
mkdir -p build && cd build

print_info "Configuring project"

cmake .. \
 -G Ninja \
 -DCMAKE_LINKER=${LD} \
 -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
 -DUSE_LIBCXX=${USE_LIBCXX} \
 -DBUILD_TESTING=${WITH_TESTING} \
 -DFORCE_BUNDLED_CV=${FORCE_BUNDLED_CV} \
 -DFORCE_BUNDLED_EIGEN=${FORCE_BUNDLED_EIGEN} \
 -DWITH_TESTING=${WITH_TESTING} \
 -DWITH_CONTRACT_EXCEPTION=${WITH_CONTRACT_EXCEPTION} \
 -DWITH_STATIC_STDCXXLIB=${WITH_STATIC_STDCXXLIB} \
 -DWITH_IPO=${WITH_IPO} \
 -DWITH_SSE42=${WITH_SSE42} \
 -DWITH_AVX2=${WITH_AVX2} \
 -DWITH_BENCHMARK=${WITH_BENCHMARK} \
 -DWITH_UBSAN=${WITH_UBSAN} \
 -DWITH_ASAN=${WITH_ASAN} \
 -DWITH_TSAN=${WITH_TSAN}

print_info "Building project"
ninja dependencies
cmake .
ninja

if [ "${WITH_TESTING}" == "ON" ]; then
    print_info "Run tests"
    ctest --output-on-failure "-j$(nproc)" -R "test_" .
fi

if [ "${WITH_BENCHMARK}" == "ON" ]; then
    print_info "Run benchmarks"
    ctest --verbose -R "bm_" .
fi

exit 0

#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Incorrect call!"
    exit 1
fi

exe="$1"
helpers="$2"

. "${helpers}"

print_info "Using \"${exe}\" as driver executable"

if ${exe} ; then
    print_error "Not enough arguments calling not checked"
    exit 1
fi

if ! ${exe} --version ; then
    print_error "Printing the version is required to work"
    exit 1
fi

print_info "Testing for bad input files"
if ${exe} --input "does-not-exist-{}.feat" \
    --start 0 --end 1 \
    keypoint-distribution \
    --image-width 960 --image-height 540 ; then
    print_error "Analyzed non-exsiting features without signaling failure"
    exit 1
fi

if ${exe} --input "does-not-exist-{}.feat" \
    --start 0 --end 1 \
    min-distance \
    --image-width 960 --image-height 540 ; then
    print_error "Analyzed non-exsiting features without signaling failure"
    exit 1
fi

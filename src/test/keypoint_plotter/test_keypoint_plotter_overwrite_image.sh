#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Incorrect call!"
    exit 1
fi

exe="$1"
helpers="$2"

. "${helpers}"

print_info "Using \"${exe}\" as driver executable"

if ! ${exe} \
    --input sift-{}.feat \
    --original-file depth-{}.png \
    --output keypoints-depth-{}.png \
    --start 0 --end 1 ; then
    print_error "Could not plot keypoint with overwritten target 16bit gray"
    exit 1
fi
if [ ! -f keypoints-depth-0.png ] || \
   [ ! -f keypoints-depth-1.png ] ; then
    print_error "Expected files not created"
    exit 1
fi

if ! ${exe} \
    --input sift-{}.feat \
    --original-file color-{}.png \
    --output keypoints-color-{}.png \
    --start 0 --end 1 ; then
    print_error "Could not plot keypoint with overwritten target 8bit rgb"
    exit 1
fi
if [ ! -f keypoints-color-0.png ] || \
   [ ! -f keypoints-color-1.png ] ; then
    print_error "Expected files not created"
    exit 1
fi

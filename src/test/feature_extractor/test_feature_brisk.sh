#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Incorrect call!"
    exit 1
fi

exe="$1"
helpers="$2"

. "${helpers}"

print_info "Using \"${exe}\" as driver executable"

print_info "Cleaning old artifacts"
rm -f brisk-*

set -v

if ! ${exe} \
   -i "flexion-{}.png" \
   -o "brisk-{}.feat" \
   -s 0 -e 1 \
   detector brisk \
   descriptor brisk ; then
    print_error "Default BRISK-Detection did not work"
    exit 1
fi
if  [ ! -f brisk-0.feat ] || \
    [ ! -f brisk-1.feat ]; then
    print_error "Did not create expected output files."
    exit 1
fi

if ! ${exe} \
   -i "flexion-{}.png" \
   -o "brisk-size-response-{}.feat" \
   -s 0 -e 1 \
   detector --kp-response-threshold 200. --kp-size-threshold 10. brisk \
   descriptor brisk ; then
    print_error "BRISK-Detection filtering by keypoint response and size did not work"
    exit 1
fi
if  [ ! -f brisk-size-response-0.feat ] || \
    [ ! -f brisk-size-response-1.feat ]; then
    print_error "Did not create expected output files."
    exit 1
fi
if  [ "$(du -k brisk-0.feat | cut -f1)" -le "$(du -k brisk-size-response-0.feat | cut -f1)" ]; then
    print_error "Filter keypoints must produce less data."
    exit 1
fi

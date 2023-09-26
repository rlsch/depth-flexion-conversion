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
rm -f akaze-*

set -v

if ! ${exe} \
   -i "flexion-{}.png" \
   -o "akaze-{}.feat" \
   -s 0 -e 1 \
   detector akaze \
   descriptor akaze ; then
    print_error "Default AKAZE-Detection did not work"
    exit 1
fi
if  [ ! -f akaze-0.feat ] || \
    [ ! -f akaze-1.feat ]; then
    print_error "Did not create expected output files."
    exit 1
fi

if ! ${exe} \
   -i "flexion-{}.png" \
   -o "akaze-size-{}.feat" \
   -s 0 -e 1 \
   detector --kp-size-threshold 5. akaze \
   descriptor akaze ; then
    print_error "AKAZE-Detection filtering by keypoint size did not work"
    exit 1
fi
if  [ ! -f akaze-size-0.feat ] || \
    [ ! -f akaze-size-1.feat ]; then
    print_error "Did not create expected output files."
    exit 1
fi
if  [ "$(du -k akaze-0.feat | cut -f1)" -le "$(du -k akaze-size-0.feat | cut -f1)" ]; then
    print_error "Filter keypoints must produce less data."
    exit 1
fi

if ! ${exe} \
   -i "flexion-{}.png" \
   -o "akaze-response-{}.feat" \
   -s 0 -e 1 \
   detector --kp-response-threshold 0.004 akaze \
   descriptor akaze ; then
    print_error "AKAZE-Detection filtering by keypoint response did not work"
    exit 1
fi
if  [ ! -f akaze-response-0.feat ] || \
    [ ! -f akaze-response-1.feat ]; then
    print_error "Did not create expected output files."
    exit 1
fi
if  [ "$(du -k akaze-0.feat | cut -f1)" -le "$(du -k akaze-response-0.feat | cut -f1)" ]; then
    print_error "Filter keypoints must produce less data."
    exit 1
fi

if ! ${exe} \
   -i "flexion-{}.png" \
   -o "akaze-size-response-{}.feat" \
   -s 0 -e 1 \
   detector --kp-response-threshold 0.004 --kp-size-threshold 5. akaze \
   descriptor akaze ; then
    print_error "AKAZE-Detection filtering by keypoint response and size did not work"
    exit 1
fi
if  [ ! -f akaze-size-response-0.feat ] || \
    [ ! -f akaze-size-response-1.feat ]; then
    print_error "Did not create expected output files."
    exit 1
fi
if  [ "$(du -k akaze-0.feat | cut -f1)" -le "$(du -k akaze-size-response-0.feat | cut -f1)" ]; then
    print_error "Filter keypoints must produce less data."
    exit 1
fi

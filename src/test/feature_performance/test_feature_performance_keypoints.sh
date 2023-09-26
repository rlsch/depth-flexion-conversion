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
    --input "sift-{}.feat" \
    --start 0 --end 1 \
    keypoint-distribution \
    --image-width 960 --image-height 540 ; then
    print_error "Could not analyze sift keypoints"
    exit 1
fi

if ! ${exe} --input "orb-{}.feat" \
    --start 0 --end 1 \
    keypoint-distribution \
    --image-width 960 --image-height 540 ; then
    print_error "Could not analyze orb keypoints"
    exit 1
fi

print_info "Test if statistics are written to a file if requested"
rm -f orb.stat
if ! ${exe} --input "orb-{}.feat" \
    --start 0 --end 1 \
    --output orb.stat \
    keypoint-distribution \
    --image-width 960 --image-height 540 ; then
    print_error "Could not analyze orb keypoints and write statistics to file"
    exit 1
fi
if [ ! -f orb.stat ] ; then
    print_error "Expected file with statistics!"
    exit 1
fi

print_info "Test if histograms are written to file properly"
rm -f orb_response.dat orb_size.dat orb_distance.dat orb_distribution.dat
if ! ${exe} --input "orb-{}.feat" \
    --start 0 --end 1 \
    keypoint-distribution \
    --image-width 960 --image-height 540 \
    --response-histo orb_response.dat \
    --size-histo orb_size.dat \
    --kp-distance-histo orb_distance.dat \
    --kp-distribution-histo orb_distribution.dat ; then
    print_error "Could not analyze orb keypoints and write statistics to file"
    exit 1
fi
if [ ! -f orb_response.dat ] || [ ! -f orb_size.dat ] ||
   [ ! -f orb_distance.dat ] || [ ! -f orb_distribution.dat ] ; then
    print_error "Expected histogram files for keypoint characteristics!"
    exit 1
fi

print_info "Testing for bad input files"
if ${exe} --input "does-not-exist-{}.feat" \
    --start 0 --end 1 \
    keypoint-distribution \
    --image-width 960 --image-height 540 ; then
    print_error "Did not signal failure for non-existing keypoints."
    exit 1
fi

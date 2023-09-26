#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Incorrect call!"
    exit 1
fi

exe="$1"
helpers="$2"

. "${helpers}"

print_info "Using \"${exe}\" as driver executable"

if ! ${exe} --input "sift-{}.feat" \
    --start 0 --end 1 \
    min-distance --norm L2 ; then
    print_error "Could not analyze sift features under L2 norm"
    exit 1
fi

if ! ${exe} --input "orb-{}.feat" \
    --start 0 --end 1 \
    min-distance --norm HAMMING ; then
    print_error "Could not analyze orb features under Hamming norm"
    exit 1
fi

print_info "Test if statistics are written to file"
rm -f orb-minimal-distance.stat
if ! ${exe} --input "orb-{}.feat" \
    --start 0 --end 1 \
    --output orb-minimal-distance.stat \
    min-distance --norm HAMMING ; then
    print_error "Could not analyze orb features under Hamming norm"
    exit 1
fi
if [ ! -f orb-minimal-distance.stat ] ; then
    print_error "Did not create file for descriptor distances!"
    exit 1
fi

print_info "Test if histogram is written"
rm -f orb_minimal_distance.dat
if ! ${exe} --input "orb-{}.feat" \
    --start 0 --end 1 \
    min-distance --norm HAMMING \
    --min-distance-histo orb_minimal_distance.dat; then
    print_error "Could not analyze orb features under Hamming norm"
    exit 1
fi
if [ ! -f orb_minimal_distance.dat ] ; then
    print_error "Did not create file for histogram of descriptor distances!"
    exit 1
fi

print_info "Testing for bad input files"
if ${exe} --input "does-not-exist-{}.feat" \
    --start 0 --end 1 \
    min-distance ; then
    print_error "Did not signal failure for non-existing descriptors."
    exit 1
fi

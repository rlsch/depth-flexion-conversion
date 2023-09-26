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

print_info "Testing that bad files are causing breakage."
if ${exe} \
    --input sift-{}.feat \
    --output keypoints-{}.not-an-image \
    --start 0 --end 1 ; then
    print_error "Non-existing file formats should not be outputted by the plotter"
    exit 1
fi
if [ -f keypoints-0.not-an-image ] ; then
    print_error "Created bad image format"
    exit 1
fi

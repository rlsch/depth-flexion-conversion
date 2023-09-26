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
rm -f surf-*

set -v

if ! ${exe} \
    -i "flexion-{}.png" -o "surf-{}.feat" \
    -s 0 -e 1 \
    detector surf descriptor surf ; then
    print_error "Default SURF-Detection did not work"
    exit 1
fi
if  [ ! -f surf-0.feat ] || \
    [ ! -f surf-1.feat ]; then
    print_error "Did not create expected output files."
    exit 1
fi

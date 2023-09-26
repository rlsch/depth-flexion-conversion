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
rm -f agast-*

set -v

if ! ${exe} \
   -i "flexion-{}.png" \
   -o "agast-{}.feat" \
   -s 0 -e 1 \
   detector agast \
   descriptor null ; then
    print_error "AGAST keypoints without descriptor did not work"
    exit 1
fi
if  [ ! -f agast-0.feat ] || \
    [ ! -f agast-1.feat ]; then
    print_error "Did not create expected output files."
    exit 1
fi

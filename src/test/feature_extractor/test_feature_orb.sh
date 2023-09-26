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
rm -f orb-*

set -v

if ! ${exe} \
   -i "flexion-{}.png" -o "orb-{}.feat" \
   -s 0 -e 1 \
   detector orb descriptor orb ; then
    print_error "Default ORB-Detection did not work"
    exit 1
fi
if  [ ! -f orb-0.feat ] || \
    [ ! -f orb-1.feat ]; then
    print_error "Did not create expected output files."
    exit 1
fi

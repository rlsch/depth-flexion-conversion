#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Incorrect call!"
    exit 1
fi

exe="$1"
helpers="$2"

. "${helpers}"

print_info "Using \"${exe}\" as driver executable"

if grep --silent "Precise Pangolin" /etc/os-release ; then
    print_warning "Skipping Tests on old linux - See #8 for more information!"
    exit 0
fi

set -v

print_info "Clearing test directory from old test result files."
rm -f batch-flexion-*

if ! ${exe} -c "kinect_intrinsic.txt" \
    -i "data{}-depth.png" \
    -s 0 -e 1 \
    flexion \
    --output "batch-flexion-{}.png"
then
    print_error "Could not create all flexion images."
    exit 1
fi

if  [ ! -f batch-flexion-0.png ] || \
    [ ! -f batch-flexion-1.png ]; then
    print_error "Did not create expected output files without leading 0."
    exit 1
fi

if ! ${exe} -c "kinect_intrinsic.txt" \
    -i "data{:03d}-depth.png" \
    --start 2 --end 4 \
    flexion \
    --output "batch-flexion-{:03d}.png"
then
    print_error "Could not create all max-curve images."
    exit 1
fi

if  [ ! -f batch-flexion-002.png ] || \
    [ ! -f batch-flexion-003.png ] || \
    [ ! -f batch-flexion-004.png ]; then
    print_error "Did not create expected output files with leading 0."
    exit 1
fi

# Test that equirectangular images are converted properly as well
if ! ${exe} \
    -m "equirectangular" \
    -c "laser_intrinsic.txt" \
    -i "laserscan-{}-depth.png" \
    -s 0 -e 1 \
    flexion \
    --output "batch-flexion-laserscan-{}.png"
then
    print_error "Could not create all flexion images."
    exit 1
fi
if  [ ! -f batch-flexion-laserscan-0.png ] || \
    [ ! -f batch-flexion-laserscan-1.png ]; then
    print_error "Did not create expected output files without leading 0."
    exit 1
fi

print_info "Test successful!"
exit 0

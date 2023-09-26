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

if ${exe} -i "flexion-{:03d}.png" -s 0 -e 1 ; then
    print_error "Subcommand required"
    exit 1
fi

if ${exe} -i "not-existing-{:03d}.png" -s 0 -e 1 -o "foo-{}.png" \
    detector sift descriptor sift ; then
    print_error "Bad filename needs to fail"
    exit 1
fi

rm -f compressed-*
if ! ${exe} -i "flexion-{}.png" -s 0 -e 1 \
     -o "compressed-{}.feat.gz" \
     detector akaze descriptor akaze ; then
    print_error "Writing compressed files failed"
    exit 1
fi
if  [ ! -f compressed-0.feat.gz ] || \
    [ ! -f compressed-1.feat.gz ] ; then
    print_error "Did not create expected output files."
    exit 1
fi

print_info "Test successful!"
exit 0

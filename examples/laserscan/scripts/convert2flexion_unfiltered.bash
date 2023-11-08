#!/bin/bash

mkdir -p flexion_unfiltered

../depth2x flexion \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range/{:04d}.png \
 -s 5 -e 5 \
 --output flexion_unfiltered/{:04d}.png



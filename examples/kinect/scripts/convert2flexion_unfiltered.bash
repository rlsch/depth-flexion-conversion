#!/bin/bash

mkdir -p flexion_unfiltered

../depth2x flexion \
 --type pinhole-depth \
 --model pinhole \
 -c intrinsic.txt \
 -i depth/data-{:04d}-depth.png \
 -s 0 -e 2 \
 --output flexion_unfiltered/flexion_{:04d}_unfiltered.png

#!/bin/bash

mkdir -p range

../depth2x range \
 -c intrinsic.txt \
 -i depth/data-{:04d}-depth.png \
 -s 0 -e 2 \
 -o range/{:04d}.png

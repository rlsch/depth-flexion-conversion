#!/bin/bash

mkdir -p depth_filtered

../depth_filter \
 --input depth/data-{:04d}-depth.png \
 --output depth_filtered/{:04d}.png \
 --start 0 --end 2 \
 median-blur --distance 5

# mkdir -p range_filtered

# ../depth_filter \
#  --input depth/data-{:04d}-depth.png \
#  --output range_filtered/{:04d}.png \
#  --start 0 --end 2 \
#  median-blur --distance 5

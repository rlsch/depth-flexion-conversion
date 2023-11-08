#!/bin/bash

mkdir -p flexion_filtered
mkdir -p flexion_5x5_filtered
mkdir -p flexion_angle_filtered
mkdir -p flexion_normalized_filtered

../depth2x flexion \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range_filtered/{:04d}.png \
 -s 5 -e 5 \
 --output flexion_filtered/flexion_{:04d}.png

../depth2x flexion_nxn \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range_filtered/{:04d}.png \
 -s 5 -e 5 \
 --output flexion_5x5_filtered/flexion_5x5_{:04d}.png \
 --neighbors 2

../depth2x flexion_angle \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range_filtered/{:04d}.png \
 -s 5 -e 5 \
 --output flexion_angle_filtered/flexion_angle_{:04d}.png

../depth2x flexion_normalized \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range_filtered/{:04d}.png \
 -s 5 -e 5 \
 --output flexion_normalized_filtered/flexion_normalized_{:04d}.png

#!/bin/bash

mkdir -p flexion
mkdir -p flexion_5x5
mkdir -p flexion_angle
mkdir -p flexion_normalized

../depth2x flexion \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/img{:04d}_0.png \
 -s 1 -e 3 \
 --output flexion/flexion_{:04d}.png

../depth2x flexion_nxn \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/img{:04d}_0.png \
 -s 1 -e 3 \
 --output flexion_5x5/flexion_5x5_{:04d}.png \
 --neighbors 2

../depth2x flexion_angle \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/img{:04d}_0.png \
 -s 1 -e 3 \
 --output flexion_angle/flexion_angle_{:04d}.png

 ../depth2x flexion_normalized \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/img{:04d}_0.png \
 -s 1 -e 3 \
 --output flexion_normalized/flexion_normalized_{:04d}.png
 
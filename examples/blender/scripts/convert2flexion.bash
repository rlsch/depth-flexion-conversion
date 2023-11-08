#!/bin/bash

mkdir -p flexion
mkdir -p flexion_7x7
mkdir -p flexion_angle
mkdir -p flexion_normalized

../depth2x flexion \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/depth_image_scene{:04d}.png \
 -s 1 -e 3 \
 --output flexion/flexion_{:04d}.png

../depth2x flexion_nxn \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/depth_image_scene{:04d}.png \
 -s 1 -e 3 \
 --output flexion_7x7/flexion_7x7_{:04d}.png \
 --neighbors 3

../depth2x flexion_angle \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/depth_image_scene{:04d}.png \
 -s 1 -e 3 \
 --output flexion_angle/flexion_angle_{:04d}.png

../depth2x flexion_normalized \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/depth_image_scene{:04d}.png \
 -s 1 -e 3 \
 --output flexion_normalized/flexion_norm_{:04d}.png

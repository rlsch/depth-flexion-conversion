#!/bin/bash

mkdir -p bearing_filtered/horizontal
mkdir -p bearing_filtered/vertical
mkdir -p bearing_filtered/diagonal
mkdir -p bearing_filtered/antidiagonal

../depth2x bearing \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range_filtered/{:04d}.png \
 -s 5 -e 5 \
 --horizontal bearing_filtered/horizontal/ba_h_{:04d}.png

../depth2x bearing \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range_filtered/{:04d}.png \
 -s 5 -e 5 \
 --vertical bearing_filtered/vertical/ba_v_{:04d}.png

../depth2x bearing \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range_filtered/{:04d}.png \
 -s 5 -e 5 \
 --diagonal bearing_filtered/diagonal/ba_d_{:04d}.png

../depth2x bearing \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range_filtered/{:04d}.png \
 -s 5 -e 5 \
 --anti-diagonal bearing_filtered/antidiagonal/ba_ad_{:04d}.png

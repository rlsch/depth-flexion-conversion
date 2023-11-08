#!/bin/bash

mkdir -p bearing_filtered/horizontal
mkdir -p bearing_filtered/vertical
mkdir -p bearing_filtered/diagonal
mkdir -p bearing_filtered/antidiagonal

../depth2x bearing \
 --type pinhole-depth \
 --model pinhole \
 -c intrinsic.txt \
 -i depth_filtered/{:04d}.png \
 -s 0 -e 2 \
 --horizontal bearing_filtered/horizontal/ba_h_{:04d}.png

../depth2x bearing \
 --type pinhole-depth \
 --model pinhole \
 -c intrinsic.txt \
 -i depth_filtered/{:04d}.png \
 -s 0 -e 2 \
 --vertical bearing_filtered/vertical/ba_v_{:04d}.png

../depth2x bearing \
 --type pinhole-depth \
 --model pinhole \
 -c intrinsic.txt \
 -i depth_filtered/{:04d}.png \
 -s 0 -e 2 \
 --diagonal bearing_filtered/diagonal/ba_d_{:04d}.png

../depth2x bearing \
 --type pinhole-depth \
 --model pinhole \
 -c intrinsic.txt \
 -i depth_filtered/{:04d}.png \
 -s 0 -e 2 \
 --anti-diagonal bearing_filtered/antidiagonal/ba_ad_{:04d}.png

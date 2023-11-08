#!/bin/bash

mkdir -p bearing_horizontal
mkdir -p bearing_vertical
mkdir -p bearing_diagonal
mkdir -p bearing_antidiagonal

../depth2x bearing \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/img{:04d}_0.png \
 -s 1 -e 3 \
 --horizontal bearing_horizontal/ba_h_{:04d}.png

../depth2x bearing \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/img{:04d}_0.png \
 -s 1 -e 3 \
 --vertical bearing_vertical/ba_v_{:04d}.png

../depth2x bearing \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/img{:04d}_0.png \
 -s 1 -e 3 \
 --diagonal bearing_diagonal/ba_d_{:04d}.png

../depth2x bearing \
 --type pinhole-range \
 --model pinhole \
 -c intrinsic.txt \
 -i range/img{:04d}_0.png \
 -s 1 -e 3 \
 --anti-diagonal bearing_antidiagonal/ba_ad_{:04d}.png

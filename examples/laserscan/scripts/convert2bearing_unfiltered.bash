#!/bin/bash

mkdir -p bearing_unfiltered/horizontal
mkdir -p bearing_unfiltered/vertical
mkdir -p bearing_unfiltered/diagonal
mkdir -p bearing_unfiltered/antidiagonal

../depth2x bearing \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range/{:04d}.png \
 -s 5 -e 5 \
 --horizontal bearing_unfiltered/horizontal/ba_h_{:04d}_unfiltered.png

../depth2x bearing \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range/{:04d}.png \
 -s 5 -e 5 \
 --vertical bearing_unfiltered/vertical/ba_v_{:04d}_unfiltered.png

../depth2x bearing \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range/{:04d}.png \
 -s 5 -e 5 \
 --diagonal bearing_unfiltered/diagonal/ba_d_{:04d}_unfiltered.png

../depth2x bearing \
 --type pinhole-range \
 --model equirectangular \
 -c intrinsic.txt \
 -i range/{:04d}.png \
 -s 5 -e 5 \
 --anti-diagonal bearing_unfiltered/antidiagonal/ba_ad_{:04d}_unfiltered.png


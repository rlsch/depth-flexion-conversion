#!/bin/bash

mkdir -p range_filtered

../depth_filter --input range/{:04d}.png --output range_filtered/{:04d}.png --start 5 --end 5 median-blur --distance 5

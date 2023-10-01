#!/bin/bash

width=150
height=150
x=470
y=460
convert flexion3x3_0018.png -crop ${width}x${height}+${x}+${y} flexion_3x3_cropped.png
convert flexion5x5_0018.png -crop ${width}x${height}+${x}+${y} flexion_5x5_cropped.png
convert flexion7x7_0018.png -crop ${width}x${height}+${x}+${y} flexion_7x7_cropped.png
convert flexion9x9_0018.png -crop ${width}x${height}+${x}+${y} flexion_9x9_cropped.png
convert flexion11x11_0018.png -crop ${width}x${height}+${x}+${y} flexion_11x11_cropped.png
convert flexion13x13_0018.png -crop ${width}x${height}+${x}+${y} flexion_13x13_cropped.png

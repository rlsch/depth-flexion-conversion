#!/usr/bin/env python3

# This script converts the raw depth data from
# https://www.ifi.uzh.ch/en/rpg/software_datasets/multifov_datasets.html
# to 16bit range data images.
# Those images can be processed with the standard pipeline.
#
# The intrinsic for those images is provided in "info/" download.

from glob import glob
import numpy as np
import cv2


def convert_to_img(depth_file):
    w = 640
    h = 480
    img = np.ndarray(shape=(h, w))

    str_data = depth_file.read()
    splitted = str_data.split()
    for i, d in enumerate(splitted):
        u, v, val = i % w, i // w, float(d) if d != "1e+12" else 0.0
        img[v, u] = 80. * val
    # print(np.amax(img))
    return img


def main():
    orig_depth_files = glob("depth/*.depth")

    for f in orig_depth_files:
        with open(f, "r") as orig_depth:
            img = convert_to_img(orig_depth)
            cv2.imwrite("range/{}.png".format(f[5:-6]),
                        np.rint(img).astype(np.uint16))


if __name__ == "__main__":
    main()

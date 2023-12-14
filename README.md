# Converting Depth Images and Point Clouds for Feature-based Pose Estimation
[Link to official paper](doi.org/10.1109/IROS55552.2023.10341758)

![Feature Images from Depth Images](docs/all-types-labeled.gif)


## What is this

Depth images are converted into a derived feature image
that allows classical computer vision algorithm to detect features, like with classical camera images.

This repository belongs to the [IROS 2023 publication "Converting Depth Images and Point Clouds for Feature-based Pose Estimation"](https://ieeexplore.ieee.org/document/10341758) by Lösch et.al (see section [How to cite](#how-to-cite)).


### Abstract
In recent years, depth sensors have become more and more affordable and have found their way into a growing amount of robotic systems. However, mono- or multi-modal sensor registration, often a necessary step for further processing, faces many challenges on raw depth images or point clouds. 

This paper presents a method of converting depth data into images capable of visualizing spatial details that are basically hidden in traditional depth images. After noise removal, a neighborhood of points forms two normal vectors whose difference is encoded into this new conversion. Compared to Bearing Angle images, our method yields brighter, higher- contrast images with more visible contours and more details. 

We tested feature-based pose estimation of both conversions in a visual odometry task and RGB-D SLAM. For all tested features, AKAZE, ORB, SIFT, and SURF, our new Flexion images yield better results than Bearing Angle images and show great potential to bridge the gap between depth data and classical computer vision. Source code is available here: [https://rlsch.github.io/depth-flexion-conversion](https://rlsch.github.io/depth-flexion-conversion).

## Examples

For in-depth usage examples of the provided tools check `docs/examples.md`.

```bash
$ ls
> .. a bunch of orthographic depth maps AND and intrinsic file ..
> depth_0000.png depth_0001.png depth_0002.png ...
> intrinsic.txt

$ cat intrinsic.txt  # example intrinsic for a kinect pinhole image
> 960 540                       # == <width> <height>
> 519.226 0.000000 479.462      # == <fx> ignored <cx>
> 0.000000 522.23 272.737       # == ignored <fy> <cy>
> 0.000000 0.000000 1.000000    # this line is ignored

$ depth2x flexion \
    --calibration intrinsic.txt \
    --input depth_{:04d}.png \
    --type pinhole-depth \
    --start 0 \
    --end 100 \
    --output flexion_{:04d}.png

$ ls
> .. still a bunch of orthographic depth maps and the intrinsic file ..
> .. with additional a bunch of flexion images ..
> flexion_0000.png flexion_0001.png flexion_0002.png ...
```

## Getting the software

### Compiling

This project tries to support many compilers and environment but is
very Linux oriented. For more information on building see the
[Compilation Reference](docs/compilation.md).

Using a `docker` container with `ubuntu:18.04` should definitly work! 


## Contributing

Bug Reports are always welcome!

## How to cite
[Link to official paper](doi.org/10.1109/IROS55552.2023.10341758)
```
@inproceedings{Losch2023,
  author    = {L{\"o}sch, Robert and Sastuba, Mark and Toth, Jonas and Jung, Bernhard},
  title     = {Converting Depth Images and Point Clouds for Feature-based Pose Estimation},
  booktitle = {2023 IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS)},
  year      = {2023},
  pages     = {3422-3428},
  doi={10.1109/IROS55552.2023.10341758}
}
```

or

R. Lösch, M. Sastuba, J. Toth and B. Jung, "Converting Depth Images and Point Clouds for Feature-based Pose Estimation," 2023 IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS), Detroit, MI, USA, 2023, pp. 3422-3428, doi: 10.1109/IROS55552.2023.10341758.



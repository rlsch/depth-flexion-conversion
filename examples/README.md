# Converting Depth Images and Point Clouds for Feature-based Pose Estimation - Examples

Here, we offer the necessary tools and example images to help you convert depth images into Flexion or Bearing Angle images on your own.

## How to Get Started

Begin by executing the following command:

```bash
./1_setup.bash
```
This script will automatically download the required binaries that where compiled with the provided [Docker image](https://github.com/rlsch/depth-flexion-conversion/blob/main/docker/ubuntu-2004/Dockerfile). Once that's done, **you can dive into the datasets and follow the instructions provided in the _local_ README files**.

## Datasets 

### Blender
<img src="blender/range/depth_image_scene0001.png" alt="Depth Image" width="200"/>

Range images of a synthetic scene featuring a cylinder, a cube-like structure, a sphere, and a monkey head.

## Kinect
<img src="kinect/depth/data-0000-depth.png" alt="Depth Image" width="400"/>

These depth images were captured using a Microsoft Kinect v2 in an underground mine.

### Laser scan
<img src="laserscan/range/0005.png" alt="Depth Image" width="800"/>

This range image comes from a dense terrestrial laser scan conducted in an underground mine.

### Zhang Dataset
<img src="https://rpg.ifi.uzh.ch/img/projects/benefit_fov_vo/urban_canyon_pinhole.png" alt="Depth Image" width="300"/>
<img src="Zhang/range_backup/img0001_0.png" alt="Depth Image" width="300"/>

These depth images are sourced from the "Multi-FoV" synthetic dataset created by 

Z. Zhang, H. Rebecq, C. Forster, and D. Scaramuzza, "Benefit of large field-of-view cameras for visual odometry," in 2016 IEEE International Conference on Robotics and Automation (ICRA), 2016.
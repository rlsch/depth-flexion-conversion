# Synthetic Blender Dataset

Here, you can easily convert synthetic range images into Flexion or Bearing Angle images. The scene is composed of various objects: a cylinder, a cube-like structure, a sphere, and a monkey head.

<img src="range/depth_image_scene0001.png" alt="Depth Image" width="400"/>

## How to Convert Depth Images

Converting depth images into Flexion and Bearing Angle images is straightforward. You have the option to perform both conversions together by running:

```bash
./convert2all.bash
```
Alternatively, you can choose to convert them separately using these commands:

```bash
./scripts/convert2flexion.bash

./scripts/convert2bearing.bash
```

### Cleaning up afterwards

If you wish to return to the initial state and clean up any generated files, you can use the following command:

```bash
./clean_up.bash
```
This command will remove all files created by the previously mentioned scripts.
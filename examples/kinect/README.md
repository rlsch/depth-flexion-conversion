# Underground Depth Images

This dataset consists of depth images captured using a Microsoft Kinect v2 in an underground mine.

<img src="depth/data-0000-depth.png" alt="Depth Image" width="300"/>

## How to Convert Depth Images

You can convert these depth images into Flexion and Bearing Angle images in one go by running:

```bash
./convert2all.bash
```
Alternatively, you have the option to perform separate conversions.

#### Separate Conversion

Before the actual conversion, consider filtering the original images using:

```bash
./scripts/filter.bash
```
This process will create a new folder with the filtered images. Once the filtering is done, you can proceed with converting the images into Flexion images using either of the following commands:

```bash
./scripts/convert2flexion_filtered.bash

./scripts/convert2flexion_unfiltered.bash
```
You can convert the images into Bearing Angle images using these commands:

```bash
./scripts/convert2bearing_filtered.bash

./scripts/convert2bearing_unfiltered.bash
```

### Cleaning up afterwards

If you wish to return to the initial state and clean up any generated files, you can use the following command:

```bash
./clean_up.bash
```
This command will remove all files created by the previously mentioned scripts.
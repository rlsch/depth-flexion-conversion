# Underground Laser Scans

This dataset comprises range images obtained from a dense terrestrial laser scan conducted in an underground mine.

<img src="range/0005.png" alt="Depth Image" width="800"/>

## How to Convert Depth Images

You can convert these range images into Flexion and Bearing Angle images in a single operation by running:

```bash
./convert2all.bash
```
Alternatively, you have the option to perform separate conversions.

#### Separate Conversion

Before the actual conversion, consider filtering the original images using:

```bash
./scripts/filter.bash
```
This step will create a new folder with the filtered images. Once the filtering is complete, you can proceed with converting the images into Flexion images using either of the following commands:

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

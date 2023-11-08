# The "Multi-FoV" synthetic dataset

This dataset contains the depth information of the original dataset [1], converted into 16-bit range images. Images from this dataset were used in Figure 10 of our paper [2].

<img src="https://rpg.ifi.uzh.ch/img/projects/benefit_fov_vo/urban_canyon_pinhole.png" alt="Depth Image" width="400"/>

## How to Convert Depth Images

You can convert the depth information into Flexion and Bearing Angle images either simultaneously by executing:

```bash
./convert2all.bash
```
or independently.

### Initial Conversion Steps

Before you can convert the depth information from the dataset, there are some preliminary considerations. The depth information is provided in text files with the extension `.depth`, and the **depth** folder contains three of them. Before this information can be used, it must be converted into images. You can achieve this with the following command:

```bash
./scripts/depth2image.bash
```
In cases where this process may not work due to dependencies (Python3 and OpenCV), we have provided pre-created images in the **range_backup** folder. The `convert2all` script will automatically use these pre-created images if the image creation process fails.

#### Separate Conversion
Once you have the range images, you can proceed with the conversion to Flexion and Bearing Angle images using the following commands:

```bash
./scripts/convert2flexion.bash

./scripts/convert2bearing.bash
```

### Clean up afterwards

If you wish to return to the initial state and clean up any generated files, you can use the following command:

```bash
./clean_up.bash
```
This command will remove all files created by the previously mentioned scripts.

# References

[1] Z. Zhang, H. Rebecq, C. Forster, and D. Scaramuzza, "Benefit of large field-of-view cameras for visual odometry," in 2016 IEEE International Conference on Robotics and Automation (ICRA), 2016.

[2] R. Lösch, M. Sastuba, J. Toth, and B. Jung, “Converting Depth Images and Point Clouds for Feature-based Pose Estimation,” in 2023 IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS), 2023.
 
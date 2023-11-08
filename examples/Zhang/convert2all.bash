#!/bin/bash

echo ""
echo "#"
echo "# Converting original depth files to images."
echo "#"
{ # try
    ./scripts/depth2image.bash
    # needs OpenCV but this is not provided in Dockerfile to keep Dockerimage small
    # should work when you have OpenCV on your system
} || { # catch
    # Use backup images instead
    echo "... using backup images."
    cp -r range_backup/* range/.
}

echo ""
echo "#"
echo "# Converting range images to Flexion images."
echo "#"
./scripts/convert2flexion.bash

echo ""
echo "#"
echo "# Converting range images to Bearing Angle images."
echo "#"
./scripts/convert2bearing.bash

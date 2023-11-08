#!/bin/bash

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

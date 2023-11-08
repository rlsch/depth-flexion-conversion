#!/bin/bash

echo "#"
echo "# Filtering original range images"
echo "#"
./scripts/filter.bash

echo ""
echo "#"
echo "# Converting filtered range images to Flexion images."
echo "#"
./scripts/convert2flexion_filtered.bash

echo ""
echo "#"
echo "# Converting UNfiltered range images to Flexion images."
echo "#"
./scripts/convert2flexion_unfiltered.bash

echo ""
echo "#"
echo "# Converting filtered range images to Bearing Angle images."
echo "#"
./scripts/convert2bearing_filtered.bash

echo ""
echo "#"
echo "# Converting UNfiltered range images to Bearing Angle images."
echo "#"
./scripts/convert2bearing_unfiltered.bash

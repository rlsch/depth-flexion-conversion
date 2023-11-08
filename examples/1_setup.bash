#!/bin/bash

command -v wget >/dev/null 2>&1 || {
    echo >&2 "I require wget but it's not installed. Aborting."; exit 1; 
}

wget https://github.com/rlsch/depth-flexion-conversion/releases/download/v1.1.0/depth2x
chmod +x depth2x

wget https://github.com/rlsch/depth-flexion-conversion/releases/download/v1.1.0/depth_filter
chmod +x depth_filter

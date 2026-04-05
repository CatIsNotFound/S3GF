#!/bin/bash

if ! command -v git &> /dev/null; then
    git submodule update --init --remote libs/SDL3 libs/SDL3_image libs/SDL3_ttf libs/SDL3_mixer libs/fmt libs/date
    ./libs/SDL3_ttf/external/download.sh
else
    echo "Error: git is NOT available. Please install Git or add it to PATH."
fi
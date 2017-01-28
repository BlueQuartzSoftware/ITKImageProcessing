#!/usr/bin/env bash

# This script enables the ITKImageProcessing plugin in DREAM.3D's CMake
# configuration, builds the plugin, then run's DREAM.3D's test suite.
# The 'run.sh' uses this script when running inside the dream3d/dream3d Docker
# image.

set -e
set -x

cd /usr/src/DREAM3D/DREAM3D-build-MinSizeRel
cmake \
  "-DDREAM3D_EXTRA_PLUGINS:STRING=ITKImageProcessing" \
  -DBUILDNAME:STRING=Plugin-ITKImageProcessing \
  .
# Builds before running ctest. Running 'ctest -D Experimental'
# builds the project, but uses all the cores
# of the CPU that are available and create memory issues
# on CircleCI.
ninja -j8
# EMMPMUnitTest should be removed. See
# https://github.com/BlueQuartzSoftware/DREAM3D/issues/540
ctest -VV -D Experimental -E EMMPMUnitTest

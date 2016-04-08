#!/usr/bin/env bash

# This script enables the ITKImageProcessing plugin in DREAM.3D's CMake
# configuration, builds the plugin, then run's DREAM.3D's test suite.
# The 'run.sh' uses this script when running inside the dream3d/dream3d Docker
# image.

set -e
set -x

cd /usr/src/DREAM3D/DREAM3D-build-Release
cmake \
  "-DDREAM3D_EXTRA_PLUGINS:STRING=ITKImageProcessing" \
  -DBUILDNAME:STRING=Plugin-ITKImageProcessing \
  .
# EMMPMUnitTest should be removed. See
# https://github.com/BlueQuartzSoftware/DREAM3D/issues/540
ctest -VV -D Experimental -E EMMPMUnitTest

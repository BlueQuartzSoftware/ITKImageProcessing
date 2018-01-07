# ITKImageProcessing #

[![Circle CI](https://circleci.com/gh/BlueQuartzSoftware/ITKImageProcessing/tree/develop.svg?style=svg)](https://circleci.com/gh/BlueQuartzSoftware/ITKImageProcessing/tree/develop)

SIMPL Plugin to perform Image Processing using the ITK (www.itk.org) framework

## Build Options ##

When building the ITKImageProcessing Plugin due to the large number of filters 
it can take a very long time to compile (30~45 minutes depending on hardware). If you want to cut that time 
down to something manageable then you can set the following CMake variable:

  ITKImageProcessingLeanAndMean=ON

which will disable all the filters **EXCEPT** the Readers and Writers.


ITKGrayscaleGeodesicDilateImage {#itkgrayscalegeodesicdilateimage}
===============================

## Group (Subgroup) ##
ITKImageProcessing (ITKImageProcessing)

## Description ##
geodesic gray scale dilation of an image

Geodesic dilation operates on a "marker" image and a "mask" image. The marker image is dilated using an elementary structuring element (neighborhood of radius one using only the face connected neighbors). The resulting image is then compared with the mask image. The output image is the pixelwise minimum of the dilated marker image and the mask image.

Geodesic dilation is run either one iteration or until convergence. In the convergence case, the filter is equivalent to "reconstruction by dilation". This filter is implemented to handle both scenarios. The one iteration case is multi-threaded. The convergence case is delegated to another instance of the same filter (but configured to run a single iteration).

The marker image must be less than or equal to the mask image (on a pixel by pixel basis).

Geodesic morphology is described in Chapter 6 of Pierre Soille's book "Morphological Image Analysis: Principles and Applications", Second Edition, Springer, 2003.

A noniterative version of this algorithm can be found in the ReconstructionByDilationImageFilter . This noniterative solution is much faster than the implementation provided here. All ITK filters that previously used GrayscaleGeodesicDiliateImageFilter as part of their implementation have been converted to use the ReconstructionByDilationImageFilter . The GrayscaleGeodesicDilateImageFilter is maintained for backward compatibility.

\see MorphologyImageFilter , GrayscaleDilateImageFilter , GrayscaleFunctionDilateImageFilter , BinaryDilateImageFilter , ReconstructionByDilationImageFilter

## Parameters ##
| Name | Type | Description |
|------|------|------|
| RunOneIteration | bool| Set/Get whether the filter should run one iteration or until convergence. When run to convergence, this filter is equivalent to "reconstruction by dilation". Default is off. |
| FullyConnected | bool| Set/Get whether the connected components are defined strictly by face connectivity or by face+edge+vertex connectivity. Default is FullyConnectedOff. For objects that are 1 pixel wide, use FullyConnectedOn. |


## Required Geometry ##
Image

## Required Objects ##
| Kind | Default Name | Type | Component Dimensions | Description |
|------|--------------|-------------|---------|-----|
| **Cell Attribute Array** | None | N/A | (1)  | Array containing input image

## Created Objects ##
| Kind | Default Name | Type | Component Dimensions | Description |
|------|--------------|-------------|---------|-----|
| **Cell Attribute Array** | None |  | (1)  | Array containing filtered image

## References ##
[1] T.S. Yoo, M. J. Ackerman, W. E. Lorensen, W. Schroeder, V. Chalana, S. Aylward, D. Metaxas, R. Whitaker. Engineering and Algorithm Design for an Image Processing API: A Technical Report on ITK - The Insight Toolkit. In Proc. of Medicine Meets Virtual Reality, J. Westwood, ed., IOS Press Amsterdam pp 586-592 (2002). 
[2] H. Johnson, M. McCormick, L. Ibanez. The ITK Software Guide: Design and Functionality. Fourth Edition. Published by Kitware Inc. 2015 ISBN: 9781-930934-28-3
[3] H. Johnson, M. McCormick, L. Ibanez. The ITK Software Guide: Introduction and Development Guidelines. Fourth Edition. Published by Kitware Inc. 2015 ISBN: 9781-930934-27-6

## License & Copyright ##

Please see the description file distributed with this plugin.

## DREAM3D Mailing Lists ##

If you need more help with a filter, please consider asking your question on the DREAM3D Users mailing list:
https://groups.google.com/forum/?hl=en#!forum/dream3d-users

set(_filterGroupName ITKImageProcessingFilters)
set(${_filterGroupName}_FILTERS_HDRS "")

#--------
# This macro must come first before we start adding any filters
SIMPL_START_FILTER_GROUP(
  ALL_FILTERS_HEADERFILE ${AllFiltersHeaderFile}
  REGISTER_KNOWN_FILTERS_FILE ${RegisterKnownFiltersFile}
  FILTER_GROUP "${_filterGroupName}"
  BINARY_DIR ${${PLUGIN_NAME}_BINARY_DIR}
  )

#---------
# List your public filters here
set(_PublicFilters
  ITKImageReader
  ITKImageWriter
  ITKImportImageStack
  ITKAbsImage
  ITKBoxMeanImage
  ITKAcosImage
  ITKAtanImage
  ITKSqrtImage
  ITKMinMaxCurvatureFlowImage
  ITKSigmoidImage
  ITKBinaryThinningImage
  ITKHMaximaImage
  ITKCurvatureAnisotropicDiffusionImage
  ITKCosImage
  ITKHMinimaImage
  ITKExpImage
  ITKBinomialBlurImage
  ITKLogImage
  ITKRescaleIntensityImage
  ITKLabelContourImage
  ITKSaltAndPepperNoiseImage
  ITKMinimumProjectionImage
  ITKTanImage
  ITKBinaryContourImage
  ITKMedianProjectionImage
  ITKShotNoiseImage
  ITKMaximumProjectionImage
  ITKSpeckleNoiseImage
  ITKShiftScaleImage
  ITKNotImage
  ITKBinaryProjectionImage
  ITKBinaryMinMaxCurvatureFlowImage
  ITKInvertIntensityImage
  ITKGrayscaleFillholeImage
  ITKAsinImage
  ITKHConvexImage
  ITKIntensityWindowingImage
  ITKGradientAnisotropicDiffusionImage
  ITKExpNegativeImage
  ITKSinImage
  ITKValuedRegionalMinimaImage
  ITKLog10Image
  ITKSquareImage
  ITKSobelEdgeDetectionImage
  ITKBilateralImage
  ITKValuedRegionalMaximaImage
  ITKRelabelComponentImage
  ITKGrayscaleGrindPeakImage
  ITKLaplacianSharpeningImage
  ITKDiscreteGaussianImage
  ITKBinaryThresholdImage
  ITKGradientMagnitudeImage
  ITKSignedMaurerDistanceMapImage
  ITKLaplacianRecursiveGaussianImage
  ITKGradientMagnitudeRecursiveGaussianImage
  ITKOtsuMultipleThresholdsImage
  ITKDoubleThresholdImage
  ITKSumProjectionImage
  ITKNormalizeImage
  ITKApproximateSignedDistanceMapImage
  ITKCurvatureFlowImage
  ITKIsoContourDistanceImage
  ITKRegionalMinimaImage
  ITKRegionalMaximaImage
  ITKSignedDanielssonDistanceMapImage
  ITKNormalizeToConstantImage
  ITKStandardDeviationProjectionImage
  ITKDanielssonDistanceMapImage
  ITKMeanProjectionImage
  ITKZeroCrossingImage
  ITKBoundedReciprocalImage
  ITKThresholdMaximumConnectedComponentsImage
  ITKSmoothingRecursiveGaussianImage
  ITKAdaptiveHistogramEqualizationImage
  ITKCastImage
  ITKMedianImage
  ITKRGBToLuminanceImage
  ITKHistogramMatchingImage
  ITKMorphologicalWatershedImage
)


#--------------
# Loop on all the filters adding each one. In this loop we default to making each filter exposed in the user
# interface in DREAM3D. If you want to have the filter compiled but NOT exposed to the user then use the next loop
foreach(f ${_PublicFilters} )
  ADD_SIMPL_FILTER(  "${PLUGIN_NAME}" "${PLUGIN_NAME}"
                        ${_filterGroupName} ${f}
                        ${${PLUGIN_NAME}_SOURCE_DIR}/Documentation/${_filterGroupName}/${f}.md 
                        TRUE
                        ${${PLUGIN_NAME}_BINARY_DIR})
endforeach()


#---------------
# This is the list of Private Filters. These filters are available from other filters but the user will not
# be able to use them from the DREAM3D user interface.
set(_PrivateFilters
  ITKImageBase
)

#-----------------
# Loop on the Private Filters adding each one to the DREAM3DLib project so that it gets compiled.
foreach(f ${_PrivateFilters} )
  ADD_SIMPL_FILTER(  "${PLUGIN_NAME}" "${PLUGIN_NAME}"
                        ${_filterGroupName} ${f}
                        ${${PLUGIN_NAME}_SOURCE_DIR}/Documentation/${_filterGroupName}/${f}.md FALSE ${${PLUGIN_NAME}_BINARY_DIR})
endforeach()

#-------------
# These are files that need to be compiled into DREAM3DLib but are NOT filters

ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} itkInPlaceImageToDream3DDataFilter.h)
ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} itkInPlaceImageToDream3DDataFilter.hxx)
ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} itkInPlaceDream3DDataToImageFilter.h)
ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} itkInPlaceDream3DDataToImageFilter.hxx)
ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} itkDream3DImage.h)
ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} itkDream3DImage.hxx)
#ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} ITKImageBase.h)
#ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} ITKImageBase.cpp)
ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} itkImportDream3DImageContainer.h)
ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} itkImportDream3DImageContainer.hxx)
ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} itkDream3DFilterInterruption.h)
ADD_SIMPL_SUPPORT_HEADER(${${PLUGIN_NAME}_SOURCE_DIR} ${_filterGroupName} Dream3DTemplateAliasMacro.h)

#---------------------
# This macro must come last after we are done adding all the filters and support files.
SIMPL_END_FILTER_GROUP(${${PLUGIN_NAME}_BINARY_DIR} "${_filterGroupName}" "${PLUGIN_NAME}")


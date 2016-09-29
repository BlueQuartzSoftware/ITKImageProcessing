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
  ITKImageProcessingFilter
  ITKImageReader
  ITKImageWriter
  ITKGaussianBlur
  ITKBinaryThreshold

# Find all filters that do not have 'dim_vec'
# for i in $(more /home/fbudin/Devel/ITKImageProcessing/Utilities/listConverted.txt); do echo $i; echo $i |sed 's/ITK//' | xargs -I{} grep dim_vec {}Filter.json; done
ITKGradientMagnitudeRecursiveGaussianImage
ITKLaplacianRecursiveGaussianImage
ITKFFTShiftImage
ITKMinMaxCurvatureFlowImage
ITKAdditiveGaussianNoiseImage
ITKSigmoidImage
ITKAtanImage
ITKBinaryThinningImage
ITKOtsuMultipleThresholdsImage
ITKAcosImage
ITKHMaximaImage
ITKAbsImage
ITKSqrtImage
ITKCurvatureAnisotropicDiffusionImage
ITKNormalizeImage
ITKDoubleThresholdImage
ITKJoinSeriesImage
ITKSumProjectionImage
ITKCosImage
ITKThresholdMaximumConnectedComponentsImage
ITKHMinimaImage
ITKApproximateSignedDistanceMapImage
ITKConfidenceConnectedImage
ITKExpImage
ITKBinomialBlurImage
ITKDiscreteGaussianImage
ITKLogImage
ITKRescaleIntensityImage
ITKLabelContourImage
ITKSaltAndPepperNoiseImage
ITKMinimumProjectionImage
ITKTanImage
ITKCurvatureFlowImage
ITKNaryMaximumImage
ITKIsoContourDistanceImage
ITKBinaryContourImage
ITKMedianProjectionImage
ITKShotNoiseImage
ITKMaximumProjectionImage
ITKSpeckleNoiseImage
ITKGradientMagnitudeImage
ITKRegionalMaximaImage
ITKShiftScaleImage
ITKNotImage
ITKSTAPLEImage
ITKLaplacianSharpeningImage
ITKBinaryThresholdProjectionImage
ITKBinaryProjectionImage
ITKBinaryMinMaxCurvatureFlowImage
ITKInvertIntensityImage
ITKRegionalMinimaImage
ITKSignedDanielssonDistanceMapImage
ITKGrayscaleFillholeImage
ITKNormalizeToConstantImage
ITKStandardDeviationProjectionImage
ITKAsinImage
ITKHConvexImage
ITKIntensityWindowingImage
ITKGradientAnisotropicDiffusionImage
ITKExpNegativeImage
ITKSinImage
ITKValuedRegionalMinimaImage
ITKDanielssonDistanceMapImage
ITKLog10Image
ITKMeanProjectionImage
ITKSquareImage
ITKZeroCrossingImage
ITKNaryAddImage
ITKSobelEdgeDetectionImage
ITKBoundedReciprocalImage
ITKBilateralImage
ITKSmoothingRecursiveGaussianImage
ITKBinaryThresholdImage
ITKValuedRegionalMaximaImage
ITKSignedMaurerDistanceMapImage
ITKRelabelComponentImage
ITKGrayscaleGrindPeakImage
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


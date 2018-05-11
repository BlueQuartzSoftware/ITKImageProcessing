set(PLUGIN_NAME "ITKImageProcessing")
set(${PLUGIN_NAME}Test_SOURCE_DIR ${${PLUGIN_NAME}_SOURCE_DIR}/Test)
set(${PLUGIN_NAME}Test_BINARY_DIR ${${PLUGIN_NAME}_BINARY_DIR}/Test)

#------------------------------------------------------------------------------
# List all the source files here. They will NOT be compiled but instead
# be directly included in the main test source file. We list them here so that
# they will show up in IDEs
set(TEST_NAMES
  ITKImageProcessingReaderTest
  ITKImageProcessingWriterTest
  ITKImageProcessingImportImageStackTest
  ImportRegisteredImageMontageTest
  ImportImageMontageTest
  ImportVectorImageStackTest
  )

if(NOT ITKImageProcessing_LeanAndMean)
  set(TEST_NAMES
    ${TEST_NAMES}
    ITKImageProcessingImageTest
    ITKAbsImageTest
    ITKAcosImageTest
    ITKAtanImageTest
    ITKSqrtImageTest
    ITKMinMaxCurvatureFlowImageTest
    ITKSigmoidImageTest
    ITKBinaryThinningImageTest
    ITKHMaximaImageTest
    ITKCurvatureAnisotropicDiffusionImageTest
    ITKCosImageTest
    ITKHMinimaImageTest
    ITKExpImageTest
    ITKLogImageTest
    ITKRescaleIntensityImageTest
    ITKLabelContourImageTest
    ITKTanImageTest
    ITKBinaryContourImageTest
    ITKShiftScaleImageTest
    ITKNotImageTest
    ITKBinaryProjectionImageTest
#    ITKBinaryMinMaxCurvatureFlowImageTest
    ITKInvertIntensityImageTest
    ITKGrayscaleFillholeImageTest
    ITKAsinImageTest
    ITKHConvexImageTest
    ITKIntensityWindowingImageTest
    ITKGradientAnisotropicDiffusionImageTest
    ITKExpNegativeImageTest
    ITKSinImageTest
    ITKValuedRegionalMinimaImageTest
    ITKLog10ImageTest
    ITKSquareImageTest
#    ITKSobelEdgeDetectionImageTest
    ITKValuedRegionalMaximaImageTest
    ITKRelabelComponentImageTest
    ITKGrayscaleGrindPeakImageTest
    ITKBinaryThresholdImageTest
    ITKGradientMagnitudeImageTest
    ITKGradientMagnitudeRecursiveGaussianImageTest
    ITKSignedMaurerDistanceMapImageTest
    ITKLaplacianRecursiveGaussianImageTest
    ITKOtsuMultipleThresholdsImageTest
    ITKDoubleThresholdImageTest
    ITKSumProjectionImageTest
    ITKApproximateSignedDistanceMapImageTest
    ITKCurvatureFlowImageTest
    ITKIsoContourDistanceImageTest
    ITKRegionalMinimaImageTest
    ITKRegionalMaximaImageTest
    ITKSignedDanielssonDistanceMapImageTest
    ITKStandardDeviationProjectionImageTest
    ITKDanielssonDistanceMapImageTest
    ITKMeanProjectionImageTest
    ITKZeroCrossingImageTest
    ITKThresholdMaximumConnectedComponentsImageTest
    ITKSmoothingRecursiveGaussianImageTest
    ITKAdaptiveHistogramEqualizationImageTest
    ITKCastImageTest
    ITKMedianImageTest
    ITKRGBToLuminanceImageTest
    ITKHistogramMatchingImageTest
    ITKMorphologicalWatershedImageTest
    ITKMorphologicalWatershedFromMarkersImageTest
    ITKThresholdImageTest
    ITKBlackTopHatImageTest
    ITKWhiteTopHatImageTest
    ITKGrayscaleErodeImageTest
    ITKBinaryDilateImageTest
    ITKGrayscaleMorphologicalOpeningImageTest
    ITKGrayscaleMorphologicalClosingImageTest
    ITKErodeObjectMorphologyImageTest
    ITKClosingByReconstructionImageTest
    ITKDilateObjectMorphologyImageTest
    ITKBinaryErodeImageTest
    ITKMorphologicalGradientImageTest
    ITKOpeningByReconstructionImageTest
    ITKBinaryMorphologicalOpeningImageTest
    ITKBinaryOpeningByReconstructionImageTest
    ITKBinaryMorphologicalClosingImageTest
    ITKBinaryClosingByReconstructionImageTest
    ITKGrayscaleDilateImageTest
    ITKMultiScaleHessianBasedObjectnessImageTest
    ITKVectorConnectedComponentImageTest
    ITKConnectedComponentImageTest
    ITKMaskImageTest
    ITKFFTNormalizedCorrelationImageTest
    ITKVectorRescaleIntensityImageTest
    ITKPatchBasedDenoisingImageTest
  )
endif()

include( ${CMP_SOURCE_DIR}/ITKSupport/IncludeITK.cmake)


list(APPEND ${PLUGIN_NAME}_LINK_LIBS Qt5::Core H5Support SIMPLib ${ITK_LIBRARIES})


#------------------------------------------------------------------------------
# Include this file from the CMP Project
include(${CMP_SOURCE_DIR}/cmpCMakeMacros.cmake)
include(${SIMPLProj_SOURCE_DIR}/Source/SIMPLib/SIMPLibMacros.cmake)


get_filename_component(${PLUGIN_NAME}_PARENT_SOURCE_DIR "${${PLUGIN_NAME}_SOURCE_DIR}" DIRECTORY)
get_filename_component(${PLUGIN_NAME}_PARENT_BINARY_DIR "${${PLUGIN_NAME}_BINARY_DIR}" DIRECTORY)

SIMPL_GenerateUnitTestFile(PLUGIN_NAME ${PLUGIN_NAME}
                           TEST_DATA_DIR ${${PLUGIN_NAME}_SOURCE_DIR}/Test/Data
                           SOURCES ${TEST_NAMES}
                           LINK_LIBRARIES ${${PLUGIN_NAME}_LINK_LIBS}
                           INCLUDE_DIRS ${${PLUGIN_NAME}_PARENT_SOURCE_DIR}
                                        ${${PLUGIN_NAME}Test_SOURCE_DIR}
                                        ${${PLUGIN_NAME}Test_BINARY_DIR}
                                        ${${PLUGIN_NAME}_PARENT_BINARY_DIR}
)

add_dependencies(${PLUGIN_NAME}UnitTest ${PLUGIN_NAME}Gui)

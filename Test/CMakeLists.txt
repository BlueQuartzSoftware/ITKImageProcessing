set(PLUGIN_NAME "ITKImageProcessing")
set(${PLUGIN_NAME}Test_SOURCE_DIR ${${PLUGIN_NAME}_SOURCE_DIR}/Test)
set(${PLUGIN_NAME}Test_BINARY_DIR ${${PLUGIN_NAME}_BINARY_DIR}/Test)

include(${SIMPLProj_SOURCE_DIR}/Source/SIMPLib/Testing/SIMPLTestFunctions.cmake)

include_directories(${${PLUGIN_NAME}_SOURCE_DIR}/Test)
include_directories(${${PLUGIN_NAME}_BINARY_DIR})
include_directories(${SIMPLProj_SOURCE_DIR}/Source)
include_directories(${SIMPLProj_BINARY_DIR})


include( ${CMP_SOURCE_DIR}/ITKSupport/IncludeITK.cmake)

#------------------------------------------------------------------------------
# Set any additional include directories here

set(TEST_TEMP_DIR ${${PLUGIN_NAME}Test_BINARY_DIR}/Test/Temp)

# Make sure the directory is created during CMake time
file(MAKE_DIRECTORY ${TEST_TEMP_DIR})


configure_file(${${PLUGIN_NAME}_SOURCE_DIR}/Test/TestFileLocations.h.in
               ${${PLUGIN_NAME}_BINARY_DIR}/Test/${PLUGIN_NAME}TestFileLocations.h @ONLY IMMEDIATE)

#------------------------------------------------------------------------------
# List all the source files here. They will NOT be compiled but instead
# be directly included in the main test source file. We list them here so that
# they will show up in IDEs
set(TEST_NAMES
  ITKImageProcessingReader
  ITKImageProcessingWriter
  ITKImageProcessingImportImageStack
  ITKImageProcessingImage
#  ITKAbsImage
#  ITKAcosImage
#  ITKAtanImage
#  ITKSqrtImage
#  ITKMinMaxCurvatureFlowImage
#  ITKSigmoidImage
#  ITKBinaryThinningImage
#  ITKHMaximaImage
#  ITKCurvatureAnisotropicDiffusionImage
#  ITKCosImage
#  ITKHMinimaImage
#  ITKExpImage
#  ITKLogImage
#  ITKRescaleIntensityImage
#  ITKLabelContourImage
#  ITKTanImage
#  ITKBinaryContourImage
#  ITKShiftScaleImage
#  ITKNotImage
#  ITKBinaryProjectionImage
#  ITKBinaryMinMaxCurvatureFlowImage
#  ITKInvertIntensityImage
#  ITKGrayscaleFillholeImage
#  ITKAsinImage
#  ITKHConvexImage
#  ITKIntensityWindowingImage
#  ITKGradientAnisotropicDiffusionImage
#  ITKExpNegativeImage
#  ITKSinImage
#  ITKValuedRegionalMinimaImage
#  ITKLog10Image
#  ITKSquareImage
#  ITKSobelEdgeDetectionImage
#  ITKValuedRegionalMaximaImage
#  ITKRelabelComponentImage
#  ITKGrayscaleGrindPeakImage
#  ITKBinaryThresholdImage
#  ITKGradientMagnitudeImage
#  ITKGradientMagnitudeRecursiveGaussianImage
#  ITKSignedMaurerDistanceMapImage
#  ITKLaplacianRecursiveGaussianImage
#  ITKOtsuMultipleThresholdsImage
#  ITKDoubleThresholdImage
#  ITKSumProjectionImage
  ITKApproximateSignedDistanceMapImage
#  ITKCurvatureFlowImage
#  ITKIsoContourDistanceImage
#  ITKRegionalMinimaImage
#  ITKRegionalMaximaImage
#  ITKSignedDanielssonDistanceMapImage
#  ITKStandardDeviationProjectionImage
#  ITKDanielssonDistanceMapImage
#  ITKMeanProjectionImage
#  ITKZeroCrossingImage
#  ITKThresholdMaximumConnectedComponentsImage
#  ITKSmoothingRecursiveGaussianImage
#  ITKAdaptiveHistogramEqualizationImage
#  ITKCastImage
#  ITKMedianImage
#  ITKRGBToLuminanceImage
#  ITKHistogramMatchingImage
#  ITKMorphologicalWatershedImage
#  ITKMorphologicalWatershedFromMarkersImage
#  ITKThresholdImage
#  ITKBlackTopHatImage
#  ITKWhiteTopHatImage
#  ITKGrayscaleErodeImage
#  ITKBinaryDilateImage
#  ITKGrayscaleMorphologicalOpeningImage
#  ITKGrayscaleMorphologicalClosingImage
#  ITKErodeObjectMorphologyImage
#  ITKClosingByReconstructionImage
#  ITKDilateObjectMorphologyImage
#  ITKBinaryErodeImage
#  ITKMorphologicalGradientImage
#  ITKOpeningByReconstructionImage
#  ITKBinaryMorphologicalOpeningImage
#  ITKBinaryOpeningByReconstructionImage
#  ITKBinaryMorphologicalClosingImage
#  ITKBinaryClosingByReconstructionImage
#  ITKGrayscaleDilateImage
#  ITKMultiScaleHessianBasedObjectnessImage
#  ITKVectorConnectedComponentImage
#  ITKConnectedComponentImage
#  ITKMaskImage
#  ITKFFTNormalizedCorrelationImage
#  ITKVectorRescaleIntensityImage
#  ITKPatchBasedDenoisingImage
  )

set( ${PLUGIN_NAME}_TEST_SRCS )
set(FilterTestIncludes "")
set(TestMainFunctors "")

foreach(name  ${TEST_NAMES})
  set( ${PLUGIN_NAME}_TEST_SRCS
    ${${PLUGIN_NAME}_TEST_SRCS}
    "${${PLUGIN_NAME}Test_SOURCE_DIR}/${name}Test.cpp"
    )
  string(CONCAT
    FilterTestIncludes
    ${FilterTestIncludes}
    "#include \"${name}Test.cpp\"\n"
    )

    string(CONCAT
      TestMainFunctors
     ${TestMainFunctors}
     "  ${name}Test()()|\n")
endforeach()

STRING(REPLACE "|" ";" TestMainFunctors ${TestMainFunctors}   )

configure_file(${SIMPLProj_SOURCE_DIR}/Source/SIMPLib/Testing/TestMain.cpp.in
               ${${PLUGIN_NAME}_BINARY_DIR}/${PLUGIN_NAME}UnitTest.cpp @ONLY)


# Set the source files properties on each source file.
foreach(f ${${PLUGIN_NAME}_TEST_SRCS})
  set_source_files_properties( ${f} PROPERTIES HEADER_FILE_ONLY TRUE)
endforeach()


list(APPEND ${PLUGIN_NAME}_LINK_LIBS Qt5::Core H5Support SIMPLib)

if(APPLE)
 list(APPEND ${PLUGIN_NAME}_LINK_LIBS SimpleITKExplicit)
else()
  list(APPEND ${PLUGIN_NAME}_LINK_LIBS ${ITK_LIBRARIES})
endif()

AddSIMPLUnitTest(TESTNAME ${PLUGIN_NAME}UnitTest
                    SOURCES ${${PLUGIN_NAME}_BINARY_DIR}/${PLUGIN_NAME}UnitTest.cpp ${${PLUGIN_NAME}_TEST_SRCS}
                    FOLDER "${PLUGIN_NAME}Plugin/Test"
                    LINK_LIBRARIES ${${PLUGIN_NAME}_LINK_LIBS} )

target_include_directories(${PLUGIN_NAME}UnitTest
                          PUBLIC ${PLUGINS_SOURCE_DIR}
                          PUBLIC ${PLUGINS_BINARY_DIR}
                          PUBLIC ${PLUGINS_BINARY_DIR}/${PLUGIN_NAME}/Test
)

if(MSVC)
  set_source_files_properties(${${PLUGIN_NAME}Test_BINARY_DIR}/${PLUGIN_NAME}UnitTest.cpp PROPERTIES COMPILE_FLAGS /bigobj)
endif()

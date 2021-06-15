set(PLUGIN_NAME "ITKImageProcessing")
set(${PLUGIN_NAME}Test_SOURCE_DIR ${${PLUGIN_NAME}_SOURCE_DIR}/Test)
set(${PLUGIN_NAME}Test_BINARY_DIR ${${PLUGIN_NAME}_BINARY_DIR}/Test)

#------------------------------------------------------------------------------
# List all the source files here. They will NOT be compiled but instead
# be directly included in the main test source file. We list them here so that
# they will show up in IDEs
set(TEST_NAMES

)

if(ITK_VERSION_MAJOR EQUAL 4)
  # Put ITK 4 Specific Modules in here
elseif(ITK_VERSION_MAJOR EQUAL 5)
  # Put ITK 5 Specific Modyles in here
  set( TEST_NAMES
      ${TEST_NAMES}
      AxioVisionV4ToTileConfigurationTest
      ImportAxioVisionV4MontageTest
      ImportZenInfoMontageTest
      ITKImportFijiMontageTest
      ITKImportRoboMetMontageTest
      ITKMedianImageTest
      ITKImportImageStackTest
      ImportVectorImageStackTest
      ITKImageProcessingWriterTest  
      ITKImageProcessingReaderTest
      EdaxEbsdMontageTest

    # ITKProxTVImageTest
  )
endif()

if(NOT ITKImageProcessing_LeanAndMean)
  set(TEST_NAMES
    ${TEST_NAMES}
    ITKAbsImageTest
    ITKAcosImageTest
    ITKAdaptiveHistogramEqualizationImageTest
    ITKApproximateSignedDistanceMapImageTest
    ITKAsinImageTest
    ITKAtanImageTest
    ITKBinaryClosingByReconstructionImageTest
    ITKBinaryContourImageTest
    ITKBinaryDilateImageTest
    ITKBinaryErodeImageTest
    ITKBinaryMorphologicalClosingImageTest
    ITKBinaryMorphologicalOpeningImageTest
    ITKBinaryOpeningByReconstructionImageTest
    ITKBinaryProjectionImageTest
    ITKBinaryThinningImageTest
    ITKBinaryThresholdImageTest
    ITKBlackTopHatImageTest
    ITKCastImageTest
    ITKClosingByReconstructionImageTest
    ITKConnectedComponentImageTest
    ITKCosImageTest
    ITKCurvatureAnisotropicDiffusionImageTest
    ITKCurvatureFlowImageTest
    ITKDanielssonDistanceMapImageTest
    ITKDilateObjectMorphologyImageTest
    ITKDoubleThresholdImageTest
    ITKErodeObjectMorphologyImageTest
    ITKExpImageTest
    ITKExpNegativeImageTest
    ITKFFTNormalizedCorrelationImageTest
    ITKGradientAnisotropicDiffusionImageTest
    ITKGradientMagnitudeImageTest
    ITKGradientMagnitudeRecursiveGaussianImageTest
    ITKGrayscaleDilateImageTest
    ITKGrayscaleErodeImageTest
    ITKGrayscaleFillholeImageTest
    ITKGrayscaleGrindPeakImageTest
    ITKGrayscaleMorphologicalClosingImageTest
    ITKGrayscaleMorphologicalOpeningImageTest
    ITKHConvexImageTest
    ITKHistogramMatchingImageTest
    ITKHMaximaImageTest
    ITKHMinimaImageTest
    ITKImageProcessingImageTest
    ITKIntensityWindowingImageTest
    ITKInvertIntensityImageTest
    ITKIsoContourDistanceImageTest
    ITKLabelContourImageTest
    ITKLaplacianRecursiveGaussianImageTest
    ITKLog10ImageTest
    ITKLogImageTest
    ITKMaskImageTest
    ITKMeanProjectionImageTest
    ITKMinMaxCurvatureFlowImageTest
    ITKMorphologicalGradientImageTest
    ITKMorphologicalWatershedFromMarkersImageTest
    ITKMorphologicalWatershedImageTest
    ITKMultiScaleHessianBasedObjectnessImageTest
    ITKNotImageTest
    ITKOpeningByReconstructionImageTest
    ITKOtsuMultipleThresholdsImageTest
    ITKPatchBasedDenoisingImageTest
    ITKRegionalMaximaImageTest
    ITKRegionalMinimaImageTest
    ITKRelabelComponentImageTest
    ITKRescaleIntensityImageTest
    ITKRGBToLuminanceImageTest
    ITKShiftScaleImageTest
    ITKSigmoidImageTest
    ITKSignedDanielssonDistanceMapImageTest
    ITKSignedMaurerDistanceMapImageTest
    ITKSinImageTest
    ITKSmoothingRecursiveGaussianImageTest
    ITKSqrtImageTest
    ITKSquareImageTest
    ITKStandardDeviationProjectionImageTest
    ITKSumProjectionImageTest
    ITKTanImageTest
    ITKThresholdImageTest
    ITKThresholdMaximumConnectedComponentsImageTest
    ITKValuedRegionalMaximaImageTest
    ITKValuedRegionalMinimaImageTest
    ITKVectorConnectedComponentImageTest
    ITKVectorRescaleIntensityImageTest
    ITKWhiteTopHatImageTest
    ITKZeroCrossingImageTest
#    ITKBinaryMinMaxCurvatureFlowImageTest
#    ITKSobelEdgeDetectionImageTest
  )
endif()

include( ${CMP_SOURCE_DIR}/ITKSupport/IncludeITK.cmake)


list(APPEND ${PLUGIN_NAME}_LINK_LIBS Qt5::Core SIMPLib ${ITK_LIBRARIES} ${PLUGIN_NAME}Server)


#------------------------------------------------------------------------------
# Include this file from the CMP Project
include(${CMP_SOURCE_DIR}/cmpCMakeMacros.cmake)
include(${SIMPLProj_SOURCE_DIR}/Source/SIMPLib/SIMPLibMacros.cmake)


get_filename_component(${PLUGIN_NAME}_PARENT_SOURCE_DIR "${${PLUGIN_NAME}_SOURCE_DIR}" DIRECTORY)
get_filename_component(${PLUGIN_NAME}_PARENT_BINARY_DIR "${${PLUGIN_NAME}_BINARY_DIR}" DIRECTORY)

SIMPL_GenerateUnitTestFile(PLUGIN_NAME ${PLUGIN_NAME}
                           TEST_DATA_DIR ${${PLUGIN_NAME}_SOURCE_DIR}/Test/Data
                           SOURCES ${TEST_NAMES}
                           LINK_LIBRARIES ${${PLUGIN_NAME}_LINK_LIBS} ${plug_target_name}
                           INCLUDE_DIRS ${${PLUGIN_NAME}_PARENT_SOURCE_DIR}
                                        ${${PLUGIN_NAME}Test_SOURCE_DIR}
                                        ${${PLUGIN_NAME}Test_BINARY_DIR}
                                        ${${PLUGIN_NAME}_PARENT_BINARY_DIR}
)

if(${${PLUGIN_NAME}_ENABLE_SCIFIO})
  target_compile_definitions(${PLUGIN_NAME}UnitTest PUBLIC "ITK_IMAGE_PROCESSING_HAVE_SCIFIO")
endif()

add_dependencies(${PLUGIN_NAME}UnitTest ${PLUGIN_NAME}Gui)

set(TEST_SCRIPT_FILE_EXT "sh")
set(EXE_EXT "")
if(WIN32)
  set(TEST_SCRIPT_FILE_EXT "bat")
  set(EXE_EXT ".exe")
endif()
# !!! NO Spaces in the following names. NO subdirectories either
# The names MUST match the names of the files on DISK including cApiTaliZation
set(PREBUILT_PIPELINE_NAMES

)
if(ITK_VERSION_MAJOR EQUAL 5)
  set(PREBUILT_PIPELINE_NAMES
    ${PREBUILT_PIPELINE_NAMES}
    "AxioVisionV4Import"
    "Fiji_Import"
    "ZenInfoImport"
    "RobometImport"
  )
endif()

set(PREBUILT_SUPPORT_DIR "${${PLUGIN_NAME}_SOURCE_DIR}/ExamplePipelines/${PLUGIN_NAME}")

foreach(test ${PREBUILT_PIPELINE_NAMES} )
  set(Example_PIPELINE_FILE "${${PLUGIN_NAME}_SOURCE_DIR}/ExamplePipelines/${PLUGIN_NAME}/${test}.json")
  set(SIMPL_CTEST_TEST_DRIVER "${${PLUGIN_NAME}Test_BINARY_DIR}/PrebuiltPipelineTests/${test}.${TEST_SCRIPT_FILE_EXT}")
  if(EXISTS "${DREAM3DProj_SOURCE_DIR}/Test/ctest_pipeline_driver.${TEST_SCRIPT_FILE_EXT}")
    configure_file(${DREAM3DProj_SOURCE_DIR}/Test/ctest_pipeline_driver.${TEST_SCRIPT_FILE_EXT} "${SIMPL_CTEST_TEST_DRIVER}" @ONLY)
    string(REPLACE "/" "_" test "${test}")
    add_test(NAME ${PLUGIN_NAME}_Example_${test} COMMAND "${SIMPL_CTEST_TEST_DRIVER}" )
  endif()
endforeach()

#------------------------------------------------------------------------------
# If Python is enabled, then enable the Python unit tests for this plugin
if(SIMPL_WRAP_PYTHON)
  set(PLUGIN_PYTHON_TESTS
  )

  include(${SIMPLProj_SOURCE_DIR}/Wrapping/Python/Binding/CreatePybind11Module.cmake)

  CreatePythonTests(PREFIX "PY_${PLUGIN_NAME}"
    INPUT_DIR ${${PLUGIN_NAME}Test_SOURCE_DIR}/Python
    TEST_NAMES ${PLUGIN_PYTHON_TESTS}
  )
endif()

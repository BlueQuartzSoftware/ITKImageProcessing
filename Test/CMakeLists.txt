set(PLUGIN_NAME "ITKImageProcessing")
set(${PLUGIN_NAME}Test_SOURCE_DIR ${${PLUGIN_NAME}_SOURCE_DIR}/Test)
set(${PLUGIN_NAME}Test_BINARY_DIR ${${PLUGIN_NAME}_BINARY_DIR}/Test)

#------------------------------------------------------------------------------
# List all the source files here. They will NOT be compiled but instead
# be directly included in the main test source file. We list them here so that
# they will show up in IDEs
set(TEST_NAMES

  # These are from ZeissImport
  ImportZenInfoMontageTest
  AxioVisionV4ToTileConfigurationTest
  ImportAxioVisionV4MontageTest

  ITKImageProcessingReaderTest
  ITKImageProcessingWriterTest
  ITKImportImageStackTest
  ImportVectorImageStackTest
  ITKMedianImageTest

)

if(ITK_VERSION_MAJOR EQUAL 4)
  # Put ITK 4 Specific Modules in here
elseif(ITK_VERSION_MAJOR EQUAL 5)
  # Put ITK 5 Specific Modyles in here
  set( TEST_NAMES
      ${TEST_NAMES}
      ITKImportFijiMontageTest
      ITKImportRoboMetMontageTest

      # ITKPCMTileRegistrationTest
      # ITKStitchMontageTest
  )
endif()

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


list(APPEND ${PLUGIN_NAME}_LINK_LIBS Qt5::Core H5Support SIMPLib ${ITK_LIBRARIES} ${PLUGIN_NAME}Server)


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
  configure_file(${DREAM3DProj_SOURCE_DIR}/Test/ctest_pipeline_driver.${TEST_SCRIPT_FILE_EXT} "${SIMPL_CTEST_TEST_DRIVER}" @ONLY)
  string(REPLACE "/" "_" test "${test}")
  add_test(NAME ${PLUGIN_NAME}_Example_${test} COMMAND "${SIMPL_CTEST_TEST_DRIVER}" )
endforeach()


#------------------------------------------------------------------------------
# If Python is enabled, then enable the Python unit tests for this plugin
if(SIMPL_ENABLE_PYTHON)
    get_property(SIMPL_ANACONDA_OUTPUT_DIR GLOBAL PROPERTY SIMPL_ANACONDA_OUTPUT_DIR)
    get_property(SIMPL_PYTHON_TEST_SCRIPT GLOBAL PROPERTY SIMPL_PYTHON_TEST_SCRIPT)
    get_property(PYTHON_SITE_PACKAGES_NAME GLOBAL PROPERTY PYTHON_SITE_PACKAGES_NAME)

    #------------------------------
    # Initialize the PYTHON_TEST_INPUT_DIR variable to point to the "Testing Directory"
    file(TO_NATIVE_PATH "${${PLUGIN_NAME}Test_SOURCE_DIR}/Python" PYTHON_TEST_INPUT_DIR)
    #------------------------------
    # These names should match the names "EXACTLY" (including capitalization).
    # NO Spaces in the names (which means no spaces in the variable names)
    set(PLUGIN_PYTHON_TESTS
    )

    set(TEST_SCRIPT_FILE_EXT "sh")
    if(WIN32)
      set(TEST_SCRIPT_FILE_EXT "bat")
    endif()
    foreach(test ${PLUGIN_PYTHON_TESTS})
        set(SIMPL_PYTHON_TEST_DRIVER "${SIMPL_ANACONDA_OUTPUT_DIR}/UnitTest/${PLUGIN_NAME}/${test}.${TEST_SCRIPT_FILE_EXT}")

        configure_file(${SIMPL_PYTHON_TEST_SCRIPT}
                        "${SIMPL_PYTHON_TEST_DRIVER}" @ONLY)

        add_test(NAME PY_${PLUGIN_NAME}_${test} COMMAND "${SIMPL_PYTHON_TEST_DRIVER}" )
    endforeach(test ${PLUGIN_PYTHON_TESTS})
    #------------------------------
    # Also setup a unit test for the base python unit test file that is generated as part
    # of the Pybind11 generated codes
    set(PYTHON_TEST_INPUT_DIR "${SIMPL_ANACONDA_OUTPUT_DIR}/UnitTest/${PLUGIN_NAME}")
    set(SIMPL_PYTHON_TEST_DRIVER "${SIMPL_ANACONDA_OUTPUT_DIR}/UnitTest/${PLUGIN_NAME}/${PLUGIN_NAME}.${TEST_SCRIPT_FILE_EXT}")
    set(test "${PLUGIN_NAME}_UnitTest")
    configure_file(${SIMPL_PYTHON_TEST_SCRIPT}  "${SIMPL_PYTHON_TEST_DRIVER}" @ONLY)
    add_test(NAME PY_${PLUGIN_NAME}_UnitTest COMMAND "${SIMPL_PYTHON_TEST_DRIVER}" )
endif()


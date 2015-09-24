#--////////////////////////////////////////////////////////////////////////////
#--
#--  Copyright (c) 2015, BlueQuartz Software
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--
#-- This code was partly written under US Air Force Contract FA8650-07-D-5800
#--
#--////////////////////////////////////////////////////////////////////////////

# project(ZeissImportTest)
set(${PLUGIN_NAME}Test_SOURCE_DIR ${${PLUGIN_NAME}_SOURCE_DIR}/Test)
set(${PLUGIN_NAME}Test_BINARY_DIR ${${PLUGIN_NAME}_BINARY_DIR}/Test)
include(${DREAM3DProj_SOURCE_DIR}/Test/DREAM3DTestFunctions.cmake)


include_directories(${${PROJECT_NAME}_SOURCE_DIR})
include_directories(${${PROJECT_NAME}_BINARY_DIR})


set(TEST_TEMP_DIR ${${PLUGIN_NAME}Test_BINARY_DIR}/Temp)
# Make sure the directory is created during CMake time
file(MAKE_DIRECTORY ${TEST_TEMP_DIR})


configure_file(${${PLUGIN_NAME}_SOURCE_DIR}/Test/TestFileLocations.h.in
               ${${PLUGIN_NAME}_BINARY_DIR}/Test/${PLUGIN_NAME}TestFileLocations.h @ONLY IMMEDIATE)


set(${PROJECT_NAME}_Link_Libs Qt5::Core H5Support SIMPLib Qt5::Gui Qt5::Widgets QtSupportLib ${ITK_LIBRARIES} )


AddDREAM3DUnitTest(TESTNAME ZeissImportFilterAvailabilityTest
                  SOURCES ${${PROJECT_NAME}_SOURCE_DIR}/Test/ZeissImportFilterAvailabilityTest.cpp
                  FOLDER "${PLUGIN_NAME}Plugin/Test"
                  LINK_LIBRARIES ${${PROJECT_NAME}_Link_Libs})

# This is an example of creating a simple unit test. Make the test name unique enough that
# another plugin would not have the same name as your test. Possible prefix the name of the
# test and/or the name of the plugin
# AddDREAM3DUnitTest(TESTNAME SomeTest
#                   SOURCES ${${PROJECT_NAME}_SOURCE_DIR}/Test/SomeTest.cpp
#                   LINK_LIBRARIES ${${PROJECT_NAME}_Link_Libs})



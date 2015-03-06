#--////////////////////////////////////////////////////////////////////////////
#--
#--  Copyright (c) 2015, BlueQuartz Software
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--
#-- This code was partly written under US Air Force Contract FA8650-07-D-5800
#--
#--////////////////////////////////////////////////////////////////////////////

project(TransformationPhaseTest)

include(${DREAM3DProj_SOURCE_DIR}/Test/DREAM3DTestFunctions.cmake)


include_directories(${${PROJECT_NAME}_SOURCE_DIR})
include_directories(${${PROJECT_NAME}_BINARY_DIR})


set(TEST_TEMP_DIR ${${PROJECT_NAME}_BINARY_DIR}/Temp)
# Make sure the directory is created during CMake time
file(MAKE_DIRECTORY ${TEST_TEMP_DIR})


configure_file(${${PROJECT_NAME}_SOURCE_DIR}/TestFileLocations.h.in
               ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}FileLocations.h @ONLY IMMEDIATE)


AddDREAM3DUnitTest(TESTNAME ZeissImportFilterAvailabilityTest 
                   SOURCES ${${PROJECT_NAME}_SOURCE_DIR}/ZeissImportFilterAvailabilityTest.cpp 
                   LINK_LIBRARIES Qt5::Core H5Support DREAM3DLib ${ITK_LIBRARIES})

# This is an example of creating a simple unit test. Make the test name unique enough that
# another plugin would not have the same name as your test. Possible prefix the name of the
# test and/or the name of the plugin
# AddDREAM3DUnitTest(TESTNAME SomeTest 
#                   SOURCES ${${PROJECT_NAME}_SOURCE_DIR}/SomeTest.cpp 
#                   LINK_LIBRARIES Qt5::Core H5Support DREAM3DLib)



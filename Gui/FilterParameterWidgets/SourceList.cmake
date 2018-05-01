set(${PLUGIN_NAME}_ParameterWidgets_HDRS "")
set(${PLUGIN_NAME}_ParameterWidgets_SRCS "")
set(${PLUGIN_NAME}_ParameterWidgets_UIS "")

# --------------------------------------------------------------------
# List the name of your custom FilterParameter Widgets in this section
set(${PLUGIN_NAME}_PARAMETER_WIDGETS
      ImportVectorImageStackWidget
)

# --------------------------------------------------------------------
# Create a File that has all the headers for the various Filter Parameter Widgets
set(AllFilterParameterWidgetsHeaderFile ${${PLUGIN_NAME}_BINARY_DIR}/Gui/FilterParameterWidgets/FilterParameterWidgets.h_tmp)
file(WRITE ${AllFilterParameterWidgetsHeaderFile} "#ifndef _${PLUGIN_NAME}_FilterParameterWidgets_H_\n#define _${PLUGIN_NAME}_FilterParameterWidgets_H_\n\n")
# --------------------------------------------------------------------
# Create a File that will register all of the DREAM3D Filter Parameter Widgets
set(RegisterKnownFilterParameterWidgetsFile ${${PLUGIN_NAME}_BINARY_DIR}/Gui/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp_tmp)
file(WRITE ${RegisterKnownFilterParameterWidgetsFile} "/* This file was AUTO GENERATED During the CMake Run. DO NOT EDIT THIS FILE. Edit the CMake files that generate this file */\n")
file(APPEND ${RegisterKnownFilterParameterWidgetsFile} "/* The CMake code that generates this file is located in ${CMAKE_CURRENT_LIST_FILE}*/\n")
file(APPEND ${RegisterKnownFilterParameterWidgetsFile} "/* This file will be directly included in another source file. */\n")
file(APPEND ${RegisterKnownFilterParameterWidgetsFile} "#include \"SVWidgetsLib/Core/FilterWidgetManager.h\"\n")
file(APPEND ${RegisterKnownFilterParameterWidgetsFile} "#include \"SVWidgetsLib/Core/PipelineFilterWidgetFactory.hpp\"\n")
file(APPEND ${RegisterKnownFilterParameterWidgetsFile} "#include \"${PLUGIN_NAME}/Gui/FilterParameterWidgets/FilterParameterWidgets.h\"\n\n")
file(APPEND ${RegisterKnownFilterParameterWidgetsFile} "\nvoid ${PLUGIN_NAME}GuiPlugin::registerFilterWidgets(FilterWidgetManager* idManager)\n{\n")


# --------------------------------------------------------------------
# Loop through all the FilterParameterWidgets that this plugin is going to compile and make available.
foreach(FPW ${${PLUGIN_NAME}_PARAMETER_WIDGETS})
  set(${PLUGIN_NAME}_ParameterWidgets_MOC_HDRS
    ${${PLUGIN_NAME}_ParameterWidgets_MOC_HDRS}
    ${${PLUGIN_NAME}_SOURCE_DIR}/Gui/FilterParameterWidgets/${FPW}.h
    )
  set(${PLUGIN_NAME}_ParameterWidgets_SRCS
    ${${PLUGIN_NAME}_ParameterWidgets_SRCS}
    ${${PLUGIN_NAME}_SOURCE_DIR}/Gui/FilterParameterWidgets/${FPW}.cpp
    )
  set(${PLUGIN_NAME}_ParameterWidgets_UIS
    ${${PLUGIN_NAME}_ParameterWidgets_UIS}
    ${${PLUGIN_NAME}_SOURCE_DIR}/Gui/FilterParameterWidgets/UI_Files/${FPW}.ui
    )

  file(APPEND  ${RegisterKnownFilterParameterWidgetsFile} "  {\n")
  file(APPEND  ${RegisterKnownFilterParameterWidgetsFile} "    PipelineFilterWidgetFactory<${FPW}>::Pointer factory = PipelineFilterWidgetFactory<${FPW}>::New();\n")
  file(APPEND  ${RegisterKnownFilterParameterWidgetsFile} "    idManager->addFilterWidgetFactory( \"${FPW}\", factory );\n")
  file(APPEND  ${RegisterKnownFilterParameterWidgetsFile} "  }\n")
  file(APPEND  ${AllFilterParameterWidgetsHeaderFile} "#include \"${PLUGIN_NAME}/Gui/FilterParameterWidgets/${FPW}.h\"\n")
endforeach()



# --------------------------------------------------------------------
# Close off the files with the proper C++
file(APPEND ${AllFilterParameterWidgetsHeaderFile} "\n#endif\n")
file(APPEND ${RegisterKnownFilterParameterWidgetsFile} "}\n")

cmpReplaceFileIfDifferent(OLD_FILE_PATH  ${${PLUGIN_NAME}_BINARY_DIR}/Gui/FilterParameterWidgets/FilterParameterWidgets.h
                          NEW_FILE_PATH  ${AllFilterParameterWidgetsHeaderFile} )
cmpReplaceFileIfDifferent(OLD_FILE_PATH  ${${PLUGIN_NAME}_BINARY_DIR}/Gui/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp
                          NEW_FILE_PATH  ${RegisterKnownFilterParameterWidgetsFile} )
set(AllFilterParameterWidgetsHeaderFile ${${PLUGIN_NAME}_BINARY_DIR}/Gui/FilterParameterWidgets/FilterParameterWidgets.h)
set(RegisterKnownFilterParameterWidgetsFile ${${PLUGIN_NAME}_BINARY_DIR}/Gui/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp)


cmp_IDE_SOURCE_PROPERTIES( "FilterParameterWidgets" "${${PLUGIN_NAME}_ParameterWidgets_MOC_HDRS}" "${${PLUGIN_NAME}_ParameterWidgets_SRCS}" "0")

cmp_IDE_GENERATED_PROPERTIES("FilterParameterWidgets/UI_Files" "${${PLUGIN_NAME}_ParameterWidgets_UIS}" "")

# --------------------------------------------------------------------
# and finally this will run moc:
# QT5_WRAP_CPP( ${PLUGIN_NAME}_ParameterWidgets_Generated_MOC_SRCS ${${PLUGIN_NAME}_ParameterWidgets_MOC_HDRS} )

# These generated moc files will be #include in the FilterWidget source file that
# are generated so we need to tell the build system to NOT compile these files
#set_source_files_properties( ${${PLUGIN_NAME}_ParameterWidgets_Generated_MOC_SRCS} PROPERTIES HEADER_FILE_ONLY TRUE)

# -- Run MOC and UIC on the necessary files
# QT5_ADD_RESOURCES( ${PLUGIN_NAME}_Generated_RC_SRCS "${DREAM3DProj_SOURCE_DIR}/Documentation/Filters/Generated_FilterDocs.qrc"  )

# --------------------------------------------------------------------
# Wrap UI files so they are AUTO UIC'ed
QT5_WRAP_UI( ${PLUGIN_NAME}_ParameterWidgets_Generated_UI_HDRS ${${PLUGIN_NAME}_ParameterWidgets_UIS} )
foreach(h ${${PLUGIN_NAME}_ParameterWidgets_Generated_UI_HDRS})
  set_property(SOURCE ${h} PROPERTY SKIP_AUTOMOC ON)
endforeach()



# --------------------------------------------------------------------
#-- Put the Qt generated files into their own group for IDEs
cmp_IDE_SOURCE_PROPERTIES( "Generated/Qt_Moc" "" "${${PLUGIN_NAME}_ParameterWidgets_Generated_MOC_SRCS}" "0")
cmp_IDE_SOURCE_PROPERTIES( "Generated/Qt_Uic" "${${PLUGIN_NAME}_ParameterWidgets_Generated_UI_HDRS}" "" "0")



set(ZeissXml_SRCS
   ${${PLUGIN_NAME}_SOURCE_DIR}/ZeissXml/ZeissMetaEntry.cpp
   ${${PLUGIN_NAME}_SOURCE_DIR}/ZeissXml/ZeissMetaFactory.cpp
   ${${PLUGIN_NAME}_SOURCE_DIR}/ZeissXml/ZeissTagMapping.cpp
   ${${PLUGIN_NAME}_SOURCE_DIR}/ZeissXml/ZeissTagsXmlSection.cpp
)


set(ZeissXml_HDRS
   ${${PLUGIN_NAME}_SOURCE_DIR}/ZeissXml/ZeissMetaEntry.h
   ${${PLUGIN_NAME}_SOURCE_DIR}/ZeissXml/ZeissMetaFactory.h
   ${${PLUGIN_NAME}_SOURCE_DIR}/ZeissXml/ZeissTagMapping.h
   ${${PLUGIN_NAME}_SOURCE_DIR}/ZeissXml/ZeissTagsXmlSection.h
   ${${PLUGIN_NAME}_SOURCE_DIR}/ZeissXml/ZeissTagMappingConstants.h
)


# -- Run MOC and UIC on the necessary files
QT5_ADD_RESOURCES( Generated_RC_SRCS ${ZeissXml_RCS} )
# this will run uic on .ui files:
QT5_WRAP_UI( Generated_UI_HDRS ${ZeissXml_UIS} )
# and finally this will run moc:
# # QT5_WRAP_CPP( Generated_MOC_SRCS ${BrukerEbsdXmlParser_MOC_HDRS} )

cmp_IDE_SOURCE_PROPERTIES( "${PLUGIN_NAME}/ZeissXml" "${ZeissXml_HDRS}" "${ZeissXml_SRCS}" "0")


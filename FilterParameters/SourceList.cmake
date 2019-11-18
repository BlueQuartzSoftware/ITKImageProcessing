
set (${PLUGIN_NAME}_FilterParameters_SRCS
    ${${PLUGIN_NAME}_SOURCE_DIR}/FilterParameters/ImportVectorImageStackFilterParameter.cpp
    ${${PLUGIN_NAME}_SOURCE_DIR}/FilterParameters/EbsdWarpPolynomialFilterParameter.cpp
)

set (${PLUGIN_NAME}_FilterParameters_HDRS
    ${${PLUGIN_NAME}_SOURCE_DIR}/FilterParameters/ImportVectorImageStackFilterParameter.h
    ${${PLUGIN_NAME}_SOURCE_DIR}/FilterParameters/EbsdWarpPolynomialFilterParameter.h
)
cmp_IDE_SOURCE_PROPERTIES( "FilterParameters" "${${PLUGIN_NAME}_FilterParameters_HDRS}" "${${PLUGIN_NAME}_FilterParameters_SRCS}" "${PROJECT_INSTALL_HEADERS}")

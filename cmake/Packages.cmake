##############################################################
#
#  This is a CMake configuration file for the project.
#  For preparing distribution packahes
#
##############################################################
#
# @author János Végh
message(HIGHLIGHTED "Configuring ${PROJECT_NAME} packages")

# Architecture: (mandatory)
IF(NOT CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
  # There is no such thing as i686 architecture on debian, you should use i386 instead
  # $ dpkg --print-architecture
  FIND_PROGRAM(DPKG_CMD dpkg)
  IF(NOT DPKG_CMD)
    MESSAGE(STATUS "Can not find dpkg in your path, default to i386.")
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
  ENDIF(NOT DPKG_CMD)
  EXECUTE_PROCESS(COMMAND "${DPKG_CMD}" --print-architecture
    OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
ENDIF(NOT CPACK_DEBIAN_PACKAGE_ARCHITECTURE)

set(CPACK_PACKAGING_INSTALL_PREFIX "$ENV{HOME}/${CPACK_PACKAGE_NAME}")
#set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/${CPACK_PACKAGE_NAME}")
#set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/${CPACK_PACKAGE_NAME}")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_PACKAGE_VENDOR "Végh, János  (Janos.Vegh at unideb.hu)")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tool for working with SystemC")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "János Végh (Janos.Vegh at unideb.hu)")
set(CPACK_COMPONENTS_IGNORE_GROUPS 1) # Ignore component groups
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Linux-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")


# ---[ Assemble distribution packages
set(CPACK_COMPONENT_APPS_DISPLAY_NAME "${PROJECT_NAME} V${PROJECT_VERSION} Applications")
set(CPACK_COMPONENT_APPS_DESCRIPTION
   "${PROJECT_NAME} V${PROJECT_VERSION}, executables of tool ${PROJECT_NAME}")

set(CPACK_COMPONENT_DOCS_DISPLAY_NAME "${PROJECT_NAME} V${PROJECT_VERSION} Documentations")
set(CPACK_COMPONENT_DOCS_DESCRIPTION
   "${PROJECT_NAME} V${PROJECT_VERSION}, documentations of tool ${PROJECT_NAME}")


if(BUILD_TESTS)
    set(CPACK_COMPONENT_TESTS_DISPLAY_NAME "${PROJECT_NAME} V${PROJECT_VERSION} Tests")
set(CPACK_COMPONENT_TESTS_DESCRIPTION
   "${PROJECT_NAME} V${PROJECT_VERSION}, unit tests of of tool ${PROJECT_NAME}")
endif()

set(CPACK_COMPONENT_SOURCES_DISPLAY_NAME "${PROJECT_NAME} V${PROJECT_VERSION} Sources")
set(CPACK_COMPONENT_SOURCES_DESCRIPTION
   "${PROJECT_NAME} V${PROJECT_VERSION}, full sources of tool ${PROJECT_NAME}")
set(CPACK_COMPONENTS_ALL apps docs tests srcs)


# ---[ Debian-specific settings
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "János Végh (Janos.Vegh at unideb.hu)") #required
set(CPACK_GENERATOR "DEB")
set(CPACK_DEB_COMPONENT_INSTALL 1) # Enable more than one package
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

###
# ---[ Use all CPack config params before this!!!
INCLUDE(CPack)

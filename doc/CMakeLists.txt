# This is the CMakeLists file for documenting the SysCperf components
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh


# Generate the Doxygen documentation
#https://tty1.net/blog/2014/cmake-doxygen_en.html

# ---[ Configuring  Doxygen and maybe chmcmd
if(BUILD_DOCS)
  if(DOXYGEN_FOUND)
    message(HIGHLIGHTED "Configuring ${PROJECT_NAME} HTML documentation")
    set(doxyfile_in ${CMAKE_SOURCE_DIR}/doc/documentation-config.doxygen.in PARENT_SCOPE)
    set(doxyfile_in ${CMAKE_SOURCE_DIR}/doc/documentation-config.doxygen.in)
    set(doxyfile ${CMAKE_BINARY_DIR}/Doxyfile)
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)
    message(HIGHLIGHTED "            ${PROJECT_NAME} All-in-one")
    add_custom_target(doc
#        DEPENDS ${PROJECT_NAME}_GUI ${PROJECT_NAME}_CLI
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
        COMMENT "Generating documentation with Doxygen"
        VERBATIM)
    if(UNIX)
      FIND_PROGRAM(DPKG_CHMCMD_FOUND chmcmd)
      if(DPKG_CHMCMD_FOUND)
        message(HIGHLIGHTED "Configuring ${PROJECT_NAME} CHM documentation")
        add_custom_target(chm
            DEPENDS doc
 	    COMMAND chmcmd index.hhp
            COMMAND mv ${PROJECT_NAME}.chm ../docs/${PROJECT_NAME}-${PROJECT_VERSION}.chm
            COMMAND chmod 755 ../docs/${PROJECT_NAME}-${PROJECT_VERSION}.chm
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
            COMMENT "Generating ${CMAKE_BINARY_DIR}/doc/${qhpcFileName} documentation with chmcmd using Doxygen output"
            VERBATIM)
        add_dependencies(chm doc)
      else()
        message(AUTHOR_WARNING "chmcmd is needed to build the .chm documentation.")
      endif(DPKG_CHMCMD_FOUND)
# Generate .qhc file
# Create a .qhcp file first
      set(qhcpFileName "${PROJECT_NAME}.qhcp")
         file(WRITE  "${CMAKE_BINARY_DIR}/doc/${qhcpFileName}" "")
        file(APPEND "${CMAKE_BINARY_DIR}/doc/${qhcpFileName}"
"<QHelpCollectionProject version=\"1.0\">
    <docFiles>
        <generate>
            <file>
                <input>index.qhp</input>
                <output>${PROJECT_NAME}.qch</output>
            </file>
        </generate>
        <register>
            <file>${PROJECT_NAME}.qch</file>
        </register>
    </docFiles>
</QHelpCollectionProject>")
# Now call collectiongenerator
        add_custom_target(qhc
            DEPENDS doc
            COMMAND qcollectiongenerator ${qhcpFileName} -o "${PROJECT_NAME}.qhc"
            COMMAND mv ${PROJECT_NAME}.qhc ../docs/${PROJECT_NAME}.qhc
            COMMAND mv ${PROJECT_NAME}.qch ../docs/${PROJECT_NAME}.qch
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
            COMMENT "Generating .qhc documentation for built-in help using Doxygen output"
            VERBATIM)
        add_dependencies(qhc doc chm)

   endif(UNIX)
  else()
     message(AUTHOR_WARNING "Doxygen is needed to build the .html documentation.")
  endif(DOXYGEN_FOUND)
endif(BUILD_DOCS)

INSTALL(FILES ${CMAKE_BINARY_DIR}/docs/${PROJECT_NAME}-${PROJECT_VERSION}.chm
${CMAKE_BINARY_DIR}/docs/${PROJECT_NAME}.qhcp -o "${PROJECT_NAME}.qhc"
        DESTINATION docs
 	COMPONENT docs)
file(GLOB_RECURSE DOCSFILES
    "*.dox"
)

file(GLOB_RECURSE IMAGEFILES
    "images/*.png"
)
  INSTALL(FILES  ${DOCSFILES} CMakeLists.txt documentation-config.doxygen.in SysCperf.bib
                 glossary.dox user-intro.dox welcome.dox
        DESTINATION doc
	COMPONENT srcs)

  INSTALL(FILES  ${IMAGEFILES}
	DESTINATION doc/images
	COMPONENT srcs)

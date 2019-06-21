# This is the main CMakeLists file of the Modules library
#
# @author János Végh

# Generate the modules library

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} 'Modules' library")

file(GLOB_RECURSE MY_SRCS
    *.cpp
)

file(GLOB_RECURSE MY_HEADERS
    include/*.h Config.h
)

include_directories( include
#       ${CMAKE_SOURCE_DIR}/ThirdParty/Y86/include
#       /opt/systemc/include
       ${CMAKE_INSTALL_PREFIX}/include
		)

                #Maybe you need something like this
ADD_LIBRARY(Modules  STATIC
   ${MY_SRCS}
   #${CMAKE_SOURCE_DIR}/ThirdParty/3rdPartySource.c
)

target_link_libraries(Modules
                      ${Caffe_LINKER_LIBS}
                      ${Pthread}
 )

set_target_properties(Modules
                      PROPERTIES OUTPUT_NAME Modules
                      )
target_include_directories(Modules PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


INSTALL(FILES  ${MY_SRCS} CMakeLists.txt
        DESTINATION modules
        COMPONENT srcs)

INSTALL(FILES  ${MY_HEADERS}
        DESTINATION modules/include
        COMPONENT srcs)


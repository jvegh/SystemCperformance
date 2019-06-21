# This is the CMakeLists file for making SystemC performance measurement tools
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

# Maybe also your need
# message(HIGHLIGHTED "Configuring ${PROJECT_NAME} third-party libraries")

message(HIGHLIGHTED "Configuring ${PROJECT_NAME}: SystemC-performance tools main programs")

# Maybe you need also
#if(YourCoundition)
#  add_subdirectory(YourMainSubdirectory)	# The specific simulator stuff
#endif(YourCoundition)


file(GLOB_RECURSE MY_SRCS
    "*.cpp"
)

# Maybe you need something similar
#file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/data/obj")

#file(GLOB MY_OBJECTS "${CMAKE_SOURCE_DIR}/Myfiles/source/*.E*")

#file(COPY ${MY_OBJECTS} DESTINATION "${CMAKE_BINARY_DIR}/data/obj")


include_directories(
        include
        ${CMAKE_SOURCE_DIR}/modules/include
#        ${CMAKE_SOURCE_DIR}/ThirdParty/XX/include
         ${SYSTEMC_HOME}/include
        ${SystemC_INCLUDE_DIRS}
)


set(CMAKE_CXX_FLAGS )
# Tell CMake to create the  executable
    message(HIGHLIGHTED "            ${PROJECT_NAME} Command line")
add_executable(${PROJECT_NAME}_base
        ${PROJECT_NAME}Base.cpp ${MY_SRCS}
        )
target_link_libraries(${PROJECT_NAME}_base
    Modules #Testbenchs
        SystemC::systemc
        ${Caffe_LINKER_LIBS}
        ${Pthread}
)


install(TARGETS ${PROJECT_NAME}_base
   RUNTIME
   DESTINATION bin
   COMPONENT apps)


#INSTALL(FILES	${MY_SRCS}
#	DESTINATION MyFiles/source
#        COMPONENT apps)



INSTALL(FILES ../CMakeLists.txt
	DESTINATION ..
	COMPONENT srcs)


#INSTALL(  FILES CMakeLists.txt
#	DESTINATION ThirdParty
#	COMPONENT srcs)


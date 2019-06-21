# http://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/
if (NOT __GTEST_INCLUDED)
  set(__GTEST_INCLUDED TRUE)

  # try the system-wide gtest first
  find_package(Gtest)
  if (GTEST_FOUND)
      set(GTEST_EXTERNAL FALSE)
  else()
    # fetch and build gtest from github
    # build directory
    set(gtest_PREFIX ${CMAKE_BINARY_DIR}/external/gtest-prefix)
    # install directory
    set(gtest_INSTALL ${CMAKE_BINARY_DIR}/external/gtest-install)

    # we build gtest statically, but want to link it into the caffe shared library
    # this requires position-independent code
    if (UNIX)
      set(GTEST_EXTRA_COMPILER_FLAGS "-fPIC")
    endif()

    set(GTEST_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${GTEST_EXTRA_COMPILER_FLAGS})
    set(GTEST_C_FLAGS ${CMAKE_C_FLAGS} ${GTEST_EXTRA_COMPILER_FLAGS})


    ExternalProject_Add(gtest
      DEPENDS ${GTEST_DEPENDS}
      PREFIX ${gtest_PREFIX}
      GIT_REPOSITORY "https://github.com/google/googletest"
 #     GIT_REPOSITORY "https://github.com/google/gtest"
      GIT_TAG "v0.3.4"
      UPDATE_COMMAND ""
      INSTALL_DIR ${gtest_INSTALL}
      CONFIGURE_COMMAND env "CFLAGS=${GTEST_C_FLAGS}" "CXXFLAGS=${GTEST_CXX_FLAGS}" ${gtest_PREFIX}/src/gtest/configure --prefix=${gtest_INSTALL} --enable-shared=no --enable-static=yes
      LOG_DOWNLOAD 1
      LOG_CONFIGURE 1
      LOG_INSTALL 1
      )

    set(GTEST_FOUND TRUE)
    set(GTEST_INCLUDE_DIRS ${gtest_INSTALL}/include)
    set(GTEST_LIBRARIES ${gtest_INSTALL}/lib/libgtest.a)
    set(GTEST_LIBRARY_DIRS ${gtest_INSTALL}/lib)
    set(GTEST_EXTERNAL TRUE)

    list(APPEND external_project_dependencies gtest)
  endif()

endif()

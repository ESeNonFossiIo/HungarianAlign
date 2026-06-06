CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(sequence-comparison CXX)

# Require C++11:
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Optional code-coverage instrumentation (gcov/lcov). Enable with
# -DENABLE_COVERAGE=ON; it must be set before the targets are defined so that
# the flags are applied to every compiled and linked object.
OPTION(ENABLE_COVERAGE "Enable coverage reporting (gcov)" OFF)
IF (ENABLE_COVERAGE)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
ENDIF()

FILE(GLOB cc_files source/*.cc)
FILE(GLOB exec main/*.cc)

INCLUDE_DIRECTORIES(include)
ADD_EXECUTABLE(sequence-comparison ${exec})

SET(_lib "sequence-comparison-lib")
ADD_LIBRARY(${_lib} SHARED ${cc_files})
TARGET_LINK_LIBRARIES(sequence-comparison ${_lib})

ADD_CUSTOM_TARGET(run
  COMMAND sequence-comparison
  DEPENDS sequence-comparison
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

OPTION(ENABLE_TESTS "Enable testing" OFF)
IF (ENABLE_TESTS STREQUAL ON)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(tests)
ENDIF()

MESSAGE( " " )
MESSAGE(STATUS "========================== STATUS: ==========================")
MESSAGE(STATUS " ENABLE_TESTS : ....... " ${ENABLE_TESTS} )
MESSAGE(STATUS " CMAKE_CXX_FLAGS : .... " ${CMAKE_CXX_FLAGS})
MESSAGE(STATUS "=============================================================")
MESSAGE( " " )

ADD_CUSTOM_TARGET(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
  )

ADD_CUSTOM_TARGET(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Release"
  )

# add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
  CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxygen/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  ADD_CUSTOM_TARGET(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
ENDIF(DOXYGEN_FOUND)

cmake_minimum_required(VERSION 2.6)

project(real_algebra)

SET(EXTRA_CXX_COMPILE_FLAGS "-std=c++11 -I./src -I./test -I/opt/local/include -O2 -Werror -Wall")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_COMPILE_FLAGS}")

SET(CXX_GMP_LINK_FLAGS "-lgmpxx -lgmp")
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${CXX_GMP_LINK_FLAGS}")

INCLUDE_DIRECTORIES(./src/)

SET(SRC_FILES ./src/polynomial.cpp
	      ./src/groebner.cpp
	      ./src/rational.cpp)

SET(TEST_FILES ./test/division_tests.cpp
	       ./test/groebner_tests.cpp
	       ./test/arithmetic_tests.cpp)

add_executable(all-tests ${TEST_FILES} ${SRC_FILES})

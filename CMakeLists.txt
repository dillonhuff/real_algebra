cmake_minimum_required(VERSION 2.6)

project(real_algebra)

SET(EXTRA_CXX_COMPILE_FLAGS "-std=c++11 -I./src -I./test -I/opt/local/include -O2 -Werror -Wall")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_COMPILE_FLAGS}")

SET(CXX_GMP_LINK_FLAGS "-lgmpxx -lgmp")
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${CXX_GMP_LINK_FLAGS}")

INCLUDE_DIRECTORIES(./include/)

SET(RALG_HEADERS ./include/ralg/polynomial.h
	      ./include/ralg/groebner.h
	      ./include/ralg/rational.h
	      ./include/ralg/matrix.h
	      ./include/ralg/projection.h
	      ./include/ralg/root_counting.h
	      ./include/ralg/cad.h
	      ./include/ralg/algorithm.h
	      ./include/ralg/check.h
	      ./include/ralg/algebraic_number.h)

SET(SRC_FILES ./src/polynomial.cpp
	      ./src/groebner.cpp
	      ./src/rational.cpp
	      ./src/matrix.cpp
	      ./src/projection.cpp
	      ./src/root_counting.cpp
	      ./src/cad.cpp
	      ./src/algebraic_number.cpp)

add_library(ralg ${SRC_FILES})

SET(TEST_FILES ./test/algebraic_number_tests.cpp
	       ./test/cad_tests.cpp
	       ./test/root_isolation_tests.cpp
	       ./test/projection_tests.cpp
	       ./test/division_tests.cpp
	       ./test/groebner_tests.cpp
	       ./test/arithmetic_tests.cpp
	       ./test/matrix_tests.cpp
	       ./test/partial_evaluation_tests.cpp)


add_executable(all-tests ${TEST_FILES})
target_link_libraries(all-tests ralg)

install(TARGETS ralg DESTINATION lib)
install(FILES ${RALG_HEADERS} DESTINATION include/ralg)

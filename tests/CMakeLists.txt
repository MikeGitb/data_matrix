cmake_minimum_required(VERSION 3.10)

########## General Settings for the whole project ############################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
	# Set warning level (CMAKE adds /W3 and msvc produces a warning, when we would just add /W4 )
	STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	add_compile_options(
		/permissive-
		/D_SCL_SECURE_NO_WARNINGS
		/D_CRT_SECURE_NO_WARNINGS
		/D_SILENCE_PARALLEL_ALGORITHMS_EXPERIMENTAL_WARNING
	)
else()
	add_compile_options(-Wall -Wextra)
endif()

########## Generate test executable ##########################################

# search for test source files (have to start with prefix test_)
file(GLOB_RECURSE TEST_FILES test_*.cpp)
add_executable(
	data_matrix_tests
	main.cpp
	${TEST_FILES}
)

# include "libs" directory for #incldue <catch2/catch2>
target_include_directories(data_matrix_tests PRIVATE libs)

find_package(Threads REQUIRED)
target_link_libraries(
	data_matrix_tests
PRIVATE
	Threads::Threads
	MBa::data_matrix
)


## Make ctest run build.
# idea taken from https://stackoverflow.com/questions/733475/cmake-ctest-make-test-doesnt-build-tests
# TODO: DOES NOT WORK with MSVC open folder (${CMAKE_COMMAND} seems to be the problem, but a plain "cmake" doesn't pass the correct incldue directories)

ADD_TEST(NAME build_data_matrix_tests COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target data_matrix_tests)

set(PARSE_CATCH_TESTS_ADD_TO_CONFIGURE_DEPENDS ON)
set(PARSE_CATCH_TESTS_ADD_TARGET_IN_TEST_NAME OFF)

include(libs/catch2/ParseAndAddCatchTests.cmake)

ParseAndAddCatchTests(data_matrix_tests)




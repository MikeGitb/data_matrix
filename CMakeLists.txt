cmake_minimum_required(VERSION 3.10)
project(data_matrix LANGUAGES CXX)

option(data_matrix_INCLUDE_TESTS "Generate targets in test directory" ON)

add_library(
	data_matrix
	INTERFACE
)

target_include_directories(
	data_matrix
INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

target_compile_features(
	data_matrix
INTERFACE
	cxx_std_17
)

if(data_matrix_INCLUDE_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

add_library(MBa::data_matrix ALIAS data_matrix)


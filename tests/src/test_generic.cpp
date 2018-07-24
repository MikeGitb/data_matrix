#include <data_matrix/data_matrix.h>

#include <catch2/catch.hpp>

#include <iostream>


TEST_CASE("say_hello", "[data_matrix_tests]")
{
	std::cout << mba::get_hello() << std::endl;
	CHECK( 1 == 1 );
}


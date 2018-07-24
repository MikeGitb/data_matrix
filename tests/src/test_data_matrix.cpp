#include <data_matrix/data_matrix.h>

#include <catch2/catch.hpp>

#include <iostream>

using namespace mba;

TEST_CASE( "Insert", "[data_matrix_tests]" )
{
	data_matrix<int, float, double, std::vector<int>> data;

	mba::column<float> row2 = data.template get<1>();
	mba::column<std::vector<int>> row4 = data.template get<3>();
	data.push_back( 1, 1.0, 2.0, {} );

	CHECK( data.get<0>()[0] == 1 );
	CHECK( data.get<1>()[0] == 1.0 );
	CHECK( data.get<2>()[0] == 2.0 );
	CHECK( data.get<3>()[0].size() == 0 );
}

TEST_CASE( "Iterate", "[data_matrix_tests]" )
{
	data_matrix<int, float, double> data;

	data.push_back( 1, 1.0f, 0.0 );
	data.push_back( 2, 1.2f, 0.0 );
	data.push_back( 3, 1.3f, 0.0 );
	data.push_back( 4, 1.4f, 0.0 );

	for( auto e : data ) {
		std::get<2>( e ) = std::get<0>( e ) * std::get<1>( e ); 
	}
	for( auto e : data ) {
		CHECK( std::get<2>( e ) == std::get<0>( e ) * std::get<1>( e ) );
	}
}

TEST_CASE( "erase", "[data_matrix_tests]" )
{
	data_matrix<int, float, double> data;

	data.push_back( 1, 1.0f, 0.0 );
	data.push_back( 2, 1.2f, 0.0 );
	data.push_back( 3, 1.3f, 0.0 );
	data.push_back( 4, 1.4f, 0.0 );

	data.erase( data.begin() + 1, data.begin() + 3 );

	CHECK( data.size() == 2 );
	for( auto e : data ) {
		CHECK( std::get<2>( e ) == std::get<0>( e ) * std::get<1>( e ) );
	}
}


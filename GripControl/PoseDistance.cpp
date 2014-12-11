#include "PoseDistance.h"

#include <iostream>
#include <fstream>

bool PoseDistance::load( const std::string& path )
{
	std::ifstream is( path.c_str(), std::ios::in );
	if( !is.is_open() )
	{
		return false;
	}

	finalize();

	unsigned int n;
	is >> n;

	initialize( n );

	unsigned int i, size = size1D( n );
	for( i=0; i < size; i++ )
	{
		is >> distance_list[ i ];
	}

	return true;
}

bool PoseDistance::save( const std::string& path )
{
	std::ofstream os( path.c_str(), std::ios::out );
	if( !os.is_open() )
	{
		return false;
	}

	os << num_frames << "\n";
	
	unsigned int i, size = size1D( num_frames );
	for( i=0; i < size; i++ )
	{
		os << distance_list[ i ] << "\n";
	}
	os.close();

	return true;
}

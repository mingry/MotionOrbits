#pragma once

#include <string>
#include <cfloat>

class PoseDistance
{
public:
	PoseDistance()
	{
		num_frames = 0;
		distance_list = 0;
	}
	virtual ~PoseDistance()
	{
		finalize();
	}

	bool load( const std::string& path );
	bool save( const std::string& path );

	inline void initialize( unsigned int n )
	{
		finalize();

		num_frames = n;
		distance_list = new float[ size1D(n) ];
	}
	inline void finalize()
	{
		delete[] distance_list;
		distance_list = 0;
		num_frames = 0;
	}
	inline void set( unsigned int i, unsigned int j, float d )
	{
		if( i >= num_frames || j >= num_frames )	return;
		if( i == j )	return;

		unsigned int index = index1D( i, j );
		distance_list[ index ] = d;
	}
	inline float get( unsigned int i, unsigned int j )
	{
		if( i >= num_frames || j >= num_frames )	return FLT_MAX;
		if( i == j )	return 0;

		unsigned int index = index1D( i, j );
		return distance_list[ index ];
	}
	inline unsigned int size1D( unsigned int n )
	{
		return n * (n-1) / 2;
	}
	inline unsigned int index1D( unsigned int i, unsigned int j )
	{
		unsigned int tmp;
		if( j > i )
		{
			tmp = i;
			i = j;
			j = tmp;
		}
		return ( num_frames*j ) - ( j*(j+1)/2 ) + ( i-j-1 );
	}
	inline unsigned int getNumFrames()
	{
		return num_frames;
	}

protected:
	unsigned int num_frames;
	float* distance_list;
};

#include "MotionCluster.h"
#include "SkeletalMotion.h"

#include <iostream>
#include <fstream>

bool MotionCluster::load( const std::string& path )
{
	std::ifstream is( path.c_str(), std::ios::in );
	if( !is.is_open() )
	{
		return false;
	}

	removeAll();

	unsigned int num_segments = 0, i;
	is >> num_segments;

	unsigned int centroid_f1, centroid_fN;
	is >> centroid_f1 >> centroid_fN;
	setCentroid( centroid_f1, centroid_fN );

	for( i=0; i < num_segments; i++ )
	{
		unsigned int f1, fN;
		is >> f1 >> fN;

		addSegment( f1, fN );
	}

	return true;
}

bool MotionCluster::save( const std::string& path )
{
	std::ofstream os( path.c_str(), std::ios::out );
	if( !os.is_open() )
	{
		return false;
	}

	unsigned int num_segments = getNumSegments();
	os << num_segments << std::endl;

	os << centroid_segment.first << " " << centroid_segment.second << std::endl;

	std::set< std::pair<unsigned int, unsigned int> >::iterator itor_s = motion_segments.begin();
	while( itor_s != motion_segments.end() )
	{
		unsigned int f1 = itor_s->first;
		unsigned int fN = itor_s->second;
		itor_s ++;

		os << f1 << " " << fN << std::endl;
	}

	return true;
}

float MotionCluster::calcSSD( SkeletalMotion* motion )
{
	if( motion_segments.empty() )
	{
		return FLT_MAX;
	}
	if( centroid_segment.first == 0 && centroid_segment.second == 0 )
	{
		return FLT_MAX;
	}
	if( motion_segments.size() == 1 )
	{
		return 0;
	}

	unsigned int c1 = centroid_segment.first;
	unsigned int cN = centroid_segment.second;
	this->sum_of_distances = 0;

	std::set< std::pair<unsigned int, unsigned int> >::iterator itor_s = motion_segments.begin();
	while( itor_s != motion_segments.end() )
	{
		unsigned int f1 = itor_s->first;
		unsigned int fN = itor_s->second;
		itor_s ++;

		if( c1 == f1 && cN == fN )	continue;

		this->sum_of_distances += SkeletalMotion::calcDistanceUniform( motion, c1, cN, motion, f1, fN );
	}

	return this->sum_of_distances;
}

float MotionCluster::calcScore( SkeletalMotion* motion )
{
	if( motion_segments.empty() )
	{
		return 0;
	}
	if( centroid_segment.first == 0 && centroid_segment.second == 0 )
	{
		return 0;
	}

	unsigned int c1 = centroid_segment.first;
	unsigned int cN = centroid_segment.second;
	unsigned int sum_of_frames = 0;
	float sum_of_errors = 0;

	std::set< std::pair<unsigned int, unsigned int> >::iterator itor_s = motion_segments.begin();
	while( itor_s != motion_segments.end() )
	{
		unsigned int f1 = itor_s->first;
		unsigned int fN = itor_s->second;
		itor_s ++;

		sum_of_frames += ( fN-f1+1 );

		if( c1 == f1 && cN == fN )	continue;

		sum_of_errors += SkeletalMotion::calcDistanceUniform( motion, c1, cN, motion, f1, fN );
	}

	unsigned int num_segments = getNumSegments();
	float E = sum_of_errors / (float)num_segments;	// mean error
	float F = (float)sum_of_frames;

	float error_decay = 5;
	this->score = 1 / ( F * exp(E) );	//F * exp( -E / error_decay );

	return this->score;
}

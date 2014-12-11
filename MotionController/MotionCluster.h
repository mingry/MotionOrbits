#pragma once

#include <vector>
#include <string>
#include <set>

class SkeletalMotion;

class MotionSegment
{
public:
	MotionSegment() { start_frame = 0; end_frame = 0; }
	MotionSegment( unsigned int f1, unsigned int fN )	{ start_frame = f1; end_frame = fN; }
	virtual ~MotionSegment() {}

	inline void setStartFrame( unsigned int f1 )	{ start_frame = f1; }
	inline void setEndFrame( unsigned int fN )		{ end_frame = fN; }

	inline unsigned int getStartFrame()	{ return start_frame; }
	inline unsigned int getEndFrame()	{ return end_frame; }

protected:
	unsigned int start_frame;
	unsigned int end_frame;
};

class MotionCluster
{
public:
	MotionCluster()	{ centroid_segment = std::make_pair(0,0); sum_of_distances = 0; }
	virtual ~MotionCluster()	{ }

	bool load( const std::string& path );
	bool save( const std::string& path );

	float calcSSD( SkeletalMotion* skeletal_motion );
	float calcScore( SkeletalMotion* skeletal_motion );

	inline void addSegment( unsigned int f1, unsigned int fN )
	{
		motion_segments.insert( std::make_pair(f1,fN) );
	}
	inline void removeSegment( unsigned int f1, unsigned int fN )
	{
		std::set< std::pair<unsigned int, unsigned int> >::iterator itor_s = motion_segments.find( std::make_pair(f1,fN) );
		if( itor_s != motion_segments.end() )
		{
			motion_segments.erase( itor_s );
		}
	}
	inline void removeAll()
	{
		motion_segments.clear();
	}
	inline unsigned int getNumSegments()
	{
		return (unsigned int)motion_segments.size();
	}
	inline std::set< std::pair<unsigned int, unsigned int> >* getSegments()	
	{ 
		return &motion_segments;
	}
	inline void setCentroid( unsigned int f1, unsigned int fN )	
	{ 
		centroid_segment.first = f1;
		centroid_segment.second = fN;
	}
	inline std::pair<unsigned int, unsigned int> getCentroid()			
	{ 
		return centroid_segment;
	}

	inline void setSSD( float d )
	{
		sum_of_distances = d;
	}
	inline float getSSD()
	{
		return sum_of_distances;
	}

	inline void setScore( float s )
	{
		score = s;
	}
	inline float getScore()
	{
		return score;
	}

protected:
	std::pair< unsigned int, unsigned int > centroid_segment;
	std::set< std::pair<unsigned int, unsigned int> > motion_segments;
	
	float sum_of_distances;
	float score;
};
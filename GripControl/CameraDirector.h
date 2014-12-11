#pragma once

#include "mathclass/position.h"
#include "mathclass/vector.h"

#include <vector>

class SkeletalMotion;
class Panel;

class CameraDirector
{
public:
	CameraDirector();
	virtual ~CameraDirector();

//	void initialize();
	void findBestConfiguration( Panel* panel );

protected:
	math::vector calcDirectionForMaxProjection( unsigned int f );
	math::vector calcDirectionForFrontFace( unsigned int f );

	void extractFeaturePoints( std::vector< unsigned int >* frames, std::vector< math::position >* feature_points );
	void extractFeatureLines( std::vector< math::position >* feature_points, unsigned int f, std::vector< std::pair<math::position,math::position> >* feature_lines );
	unsigned int getNumFeaturePointsPerPose();

	void transformFeaturePoints( std::vector< math::position >* feature_points, math::position* o, math::vector* v0, math::vector* v1, math::vector* v2, std::vector< math::position >* proj_points );
	void projectFeaturePoints( std::vector< math::position >* feature_points, float* view_matrix, float* proj_matrix, std::vector< math::position >* projected_points );
	void calcFrameForMaxProjection( std::vector< math::position >* feature_points, math::position* o, math::vector* v0, math::vector* v1, math::vector* v2 );

	void evalBoundingBox( std::vector< math::position >* transformed_points, float* min_x, float* max_x, float* min_y, float* max_y, float* min_z, float* max_z, unsigned int i, unsigned int j );
	void evalBoundingRectangle( std::vector< math::position >* projected_points, float* min_x, float* max_x, float* min_y, float* max_y, unsigned int i, unsigned int j );

	float evalPoseOverlapRatio( std::vector< math::position >* projected_points, unsigned int main_index );
	float evalPoseVisibility( std::vector< math::position >* projected_points, unsigned int main_index );
	float evalPoseArrangement( std::vector< math::position >* projected_points );

	float checkPoseOverlap( std::vector< math::position >* projected_points, unsigned int i, unsigned int j );
	float checkPoseOverlap( std::vector< std::pair<math::position, math::position> >* pose_i, std::vector< std::pair<math::position, math::position> >* pose_j );
	bool isPoseOverlapped( std::vector< std::pair<math::position, math::position> >* pose_i, std::vector< std::pair<math::position, math::position> >* pose_j );
	bool isLineOverlapped( const math::position& si, const math::position& ei, const math::position& sj, const math::position& ej );

	void selectPoses( std::vector< math::position >* projected_points, unsigned int main_index, std::vector< unsigned int >* pose_indices );

	Panel* panel;
};

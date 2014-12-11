#include "MotionCodec.h"
#include "SkeletalMotion.h"
#include "Skeleton.h"
#include "Joint.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>


MotionCodec::MotionCodec()
{
	original_motion = 0;
	compressed_motion = 0;
	decompressed_motion = 0;

	original_num_frames = 0;
	compressed_num_frames = 0;
	sampling_interval = 0;

	pelvis = 0;
	left_hand = 0;
	right_hand = 0;
	left_foot = 0;
	right_foot = 0;
}

MotionCodec::~MotionCodec()
{
	clear();
}

void MotionCodec::clear()
{
	delete original_motion;
	delete compressed_motion;
	delete decompressed_motion;

	original_motion = 0;
	compressed_motion = 0;
	decompressed_motion = 0;	

	std::vector< MotionCluster* >::iterator itor_c = cluster_array.begin();
	while( itor_c != cluster_array.end() )
	{
		MotionCluster* c = ( *itor_c ++ );
		delete c;
	}
	cluster_array.clear();

	std::vector< CompressedBlock* >::iterator itor_b = block_list.begin();
	while( itor_b != block_list.end() )
	{
		CompressedBlock* b = ( *itor_b ++ );
		delete b;
	}
	block_list.clear();

	original_num_frames = 0;
	compressed_num_frames = 0;
	sampling_interval = 0;

	pelvis = 0;
	left_hand = 0;
	right_hand = 0;
	left_foot = 0;
	right_foot = 0;
}

static inline bool sort_clusters( MotionCluster* lhs, MotionCluster* rhs )
{
	return lhs->getSSD() < rhs->getSSD();
}

bool MotionCodec::encode( const std::string& path_original_motion, const std::string& path_cluster_array, unsigned int num_classes, unsigned int sampling_interval )
{
	clear();

	original_motion = new SkeletalMotion;
	bool is_motion_loaded = original_motion->importFromBVH( path_original_motion );
	if( !is_motion_loaded )
	{
		clear();
		return false;
	}

	std::ifstream is( path_cluster_array.c_str(), std::ios::in );
	if( !is.is_open() )
	{
		clear();
		return false;
	}

	unsigned int num_clusters = 0, i;
	is >> num_clusters;

	for( i=0; i < num_clusters; i++ )
	{
		std::string path_cluster;
		is >> path_cluster;

		MotionCluster* c = new MotionCluster;
		cluster_array.push_back( c );

		bool is_cluster_loaded = c->load( path_cluster );
		if( !is_cluster_loaded )
		{
			clear();
			return false;
		}
	}

	is.close();
	
	//
	std::sort( cluster_array.begin(), cluster_array.end(), sort_clusters );

	unsigned int num_frames = 0;
	for( i=0; i < num_classes; i++ )
	{
		std::pair<unsigned int, unsigned int> centroid = cluster_array[ i ]->getCentroid();
		unsigned int f1 = centroid.first;
		unsigned int fN = centroid.second;
		num_frames += ( fN - f1 + 1 );
	}

	for( ; i < num_clusters; i++ )
	{
		std::set< std::pair<unsigned int, unsigned int> >* segments = cluster_array[ i ]->getSegments();
		std::set< std::pair<unsigned int, unsigned int> >::iterator itor_s = segments->begin();
		while( itor_s != segments->end() )
		{
			std::pair<unsigned int, unsigned int> segment = ( *itor_s ++ );
			unsigned int f1 = segment.first;
			unsigned int fN = segment.second;
			num_frames += ( fN - f1 + 1 );
		}
	}

	compressed_motion = new SkeletalMotion;
	compressed_motion->initialize( original_motion->getSkeleton(), num_frames );

	num_frames = 0;
	for( i=0; i < num_classes; i++ )
	{
		std::pair<unsigned int, unsigned int> centroid = cluster_array[ i ]->getCentroid();
		unsigned int f1 = centroid.first;
		unsigned int fN = centroid.second;
		compressed_motion->copyFrom( original_motion, f1, fN, math::identity_transq, num_frames );

		std::set< std::pair<unsigned int, unsigned int> >* segments = cluster_array[ i ]->getSegments();
		std::set< std::pair<unsigned int, unsigned int> >::iterator itor_s = segments->begin();
		while( itor_s != segments->end() )
		{
			std::pair<unsigned int, unsigned int> segment = ( *itor_s ++ );
			unsigned int s = segment.first;
			unsigned int e = segment.second;

			CompressedBlock* block = new CompressedBlock;
			block_list.push_back( block );

			block->original_f1 = s;
			block->original_fN = e;

			block->compressed_f1 = num_frames;
			block->compressed_fN = num_frames + ( fN - f1 + 1 );

			if( s == f1 && e == fN )
			{
				block->is_lossy = false;
			}
			else
			{
				block->is_lossy = true;

				unsigned int f = s;
				while( f <= e )
				{
					math::transq pT, lhT, rhT, lfT, rfT;
					pT = original_motion->getGlobalTransform( f, pelvis->getIndex() );
					lhT = original_motion->getGlobalTransform( f, left_hand->getIndex() );
					rhT = original_motion->getGlobalTransform( f, right_hand->getIndex() );
					lfT = original_motion->getGlobalTransform( f, left_foot->getIndex() );
					rfT = original_motion->getGlobalTransform( f, right_foot->getIndex() );

					block->pelvis_T.push_back( pT );
					block->lh_T.push_back( lhT );
					block->rh_T.push_back( rhT );
					block->lf_T.push_back( lfT );
					block->rf_T.push_back( rfT );

					f += sampling_interval;
					if( f > e && f < e + sampling_interval )
					{
						f = e;
					}
				}
			}
		}
		num_frames += ( fN - f1 + 1 );
	}

	for( ; i < num_clusters; i++ )
	{
		std::set< std::pair<unsigned int, unsigned int> >* segments = cluster_array[ i ]->getSegments();
		std::set< std::pair<unsigned int, unsigned int> >::iterator itor_s = segments->begin();
		while( itor_s != segments->end() )
		{
			std::pair<unsigned int, unsigned int> segment = ( *itor_s ++ );
			unsigned int s = segment.first;
			unsigned int e = segment.second;

			compressed_motion->copyFrom( original_motion, s, e, math::identity_transq, num_frames );

			CompressedBlock* block = new CompressedBlock;
			block_list.push_back( block );

			block->original_f1 = s;
			block->original_fN = e;

			block->compressed_f1 = num_frames;
			block->compressed_fN = num_frames + ( e - s + 1 );

			block->is_lossy = false;

			num_frames += ( e - s + 1 );
		}
	}

	this->original_num_frames = original_motion->getNumFrames();
	this->compressed_num_frames = compressed_motion->getNumFrames();
	this->sampling_interval = sampling_interval;
}

bool MotionCodec::decode( const std::string& path_compressed_motion, const std::string& path_block )
{
	clear();

	bool is_loaded = loadEncodedMotion( path_compressed_motion, path_block );
	if( !is_loaded )
	{
		return false;
	}

	Skeleton* skeleton = compressed_motion->getSkeleton();
	unsigned int num_joints = skeleton->getNumJoints();

	decompressed_motion = new SkeletalMotion;
	decompressed_motion->initialize( skeleton, original_num_frames );

	std::vector< CompressedBlock* >::iterator itor_b = block_list.begin();
	while( itor_b != block_list.end() )
	{
		CompressedBlock* block = ( *itor_b ++ );

		if( block->is_lossy )
		{
			unsigned int c_f1 = block->compressed_f1;
			unsigned int c_fN = block->compressed_fN;
			unsigned int c_len = c_fN - c_f1 + 1;

			unsigned int o_f1 = block->original_f1;
			unsigned int o_fN = block->original_fN;
			unsigned int o_len = o_fN - o_f1 + 1;

			SkeletalMotion* warp_motion = new SkeletalMotion;
			warp_motion->initialize( skeleton, o_len );

			float ratio = c_len / o_len, u = 0;
			unsigned int f, j;

			for( f=0; f < o_len; f++, u += ratio )
			{
				unsigned int o_f = o_f1 + f;
				unsigned int c_f_left = (unsigned int)( (float)c_f1 + (float)f * u );
				unsigned int c_f_right = c_f_left + 1;
				float v = ( (float)c_f1 + (float)f * u ) - (float)c_f_left; 

				for( j=0; j < num_joints; j++ )
				{
					/*
					if( j == 0 )
					{
						math::transq T = 
					}
					else
					{
						math::transq T_left = compressed_motion->getGlobalTransform( c_f_left, j );
						math::transq T_right = compressed_motion->getGlobalTransform( c_f_right, j );
						math::transq T = math::interpolate( v, T_left, T_right );
					}
					*/
				}
			}
		}
		else
		{
			decompressed_motion->copyFrom( compressed_motion, block->compressed_f1, block->compressed_fN, math::identity_transq, block->original_f1 );
		}
	}
}

bool MotionCodec::loadEncodedMotion( const std::string& path_compressed_motion, const std::string& path_block )
{
	clear();

	compressed_motion = new SkeletalMotion;
	bool is_loaded = compressed_motion->importFromBVH( path_compressed_motion );
	if( !is_loaded )
	{
		clear();
		return false;
	}

	std::ifstream is( path_block.c_str(), std::ios::in );
	if( !is.is_open() )
	{
		clear();
		return false;
	}

	is >> this->original_num_frames;
	is >> this->compressed_num_frames;
	is >> this->sampling_interval;

	assert( this->compressed_num_frames == compressed_motion->getNumFrames() );

	unsigned int num_blocks = 0, i, j;
	is >> num_blocks;

	for( i=0; i < num_blocks; i++ )
	{
		CompressedBlock* block = new CompressedBlock;
		block_list.push_back( block );

		is >> block->original_f1 >> block->original_fN;
		is >> block->compressed_f1 >> block->compressed_fN;

		unsigned int is_lossy;
		is >> is_lossy;
		block->is_lossy = ( is_lossy ? true : false );

		if( block->is_lossy )
		{
			unsigned int num_pelvis_samples = 0;
			is >> num_pelvis_samples;

			for( j=0; j < num_pelvis_samples; j++ )
			{
				float tx, ty, tz, qw, qx, qy, qz;
				is >> tx >> ty >> tz >> qw >> qx >> qy >> qz;

				math::transq T( math::quater( qw, qx, qy, qz ), math::vector( tx, ty, tz ) );
				block->pelvis_T.push_back( T );
			}

			unsigned int num_lh_samples = 0;
			is >> num_lh_samples;

			for( j=0; j < num_lh_samples; j++ )
			{
				float tx, ty, tz, qw, qx, qy, qz;
				is >> tx >> ty >> tz >> qw >> qx >> qy >> qz;

				math::transq T( math::quater( qw, qx, qy, qz ), math::vector( tx, ty, tz ) );
				block->lh_T.push_back( T );
			}

			unsigned int num_rh_samples = 0;
			is >> num_rh_samples;

			for( j=0; j < num_rh_samples; j++ )
			{
				float tx, ty, tz, qw, qx, qy, qz;
				is >> tx >> ty >> tz >> qw >> qx >> qy >> qz;

				math::transq T( math::quater( qw, qx, qy, qz ), math::vector( tx, ty, tz ) );
				block->rh_T.push_back( T );
			}

			unsigned int num_lf_samples = 0;
			is >> num_lf_samples;

			for( j=0; j < num_lf_samples; j++ )
			{
				float tx, ty, tz, qw, qx, qy, qz;
				is >> tx >> ty >> tz >> qw >> qx >> qy >> qz;

				math::transq T( math::quater( qw, qx, qy, qz ), math::vector( tx, ty, tz ) );
				block->lf_T.push_back( T );
			}

			unsigned int num_rf_samples = 0;
			is >> num_rf_samples;

			for( j=0; j < num_rf_samples; j++ )
			{
				float tx, ty, tz, qw, qx, qy, qz;
				is >> tx >> ty >> tz >> qw >> qx >> qy >> qz;

				math::transq T( math::quater( qw, qx, qy, qz ), math::vector( tx, ty, tz ) );
				block->rf_T.push_back( T );
			}
		}
	}
	return true;
}	

bool MotionCodec::saveEncodedMotion( const std::string& path_compressed_motion, const std::string& path_block )
{
	if( !compressed_motion )
	{
		return false;
	}

	bool is_saved = compressed_motion->exportIntoBVH( path_compressed_motion );
	if( !is_saved )
	{
		return false;
	}

	std::ofstream os( path_block.c_str(), std::ios::out );
	if( !os.is_open() )
	{
		return false;
	}

	os << original_num_frames << "\n";
	os << compressed_num_frames << "\n";
	os << sampling_interval << "\n";

	unsigned int num_blocks = (unsigned int)block_list.size();
	os << num_blocks << "\n\n";

	std::vector< CompressedBlock* >::iterator itor_b = block_list.begin();
	while( itor_b != block_list.end() )
	{
		CompressedBlock* block = ( *itor_b ++ );
		os << block->original_f1 << "\t" << block->original_fN << "\n";
		os << block->compressed_f1 << "\t" << block->compressed_fN << "\n";
		os << ( block->is_lossy ? 1 : 0 ) << "\n";

		if( block->is_lossy )
		{
			unsigned int i;

			unsigned int num_pelvis_samples = (unsigned int)block->pelvis_T.size();
			os << num_pelvis_samples << "\n";

			for( i=0; i < num_pelvis_samples; i++ )
			{
				float tx = block->pelvis_T[ i ].translation.x();
				float ty = block->pelvis_T[ i ].translation.y();
				float tz = block->pelvis_T[ i ].translation.z();

				float qw = block->pelvis_T[ i ].rotation.w();
				float qx = block->pelvis_T[ i ].rotation.x();
				float qy = block->pelvis_T[ i ].rotation.y();
				float qz = block->pelvis_T[ i ].rotation.z();

				os << tx << "\t" << ty << "\t" << tz << "\t" << qw << "\t" << qx << "\t" << qy << "\t" << qz << "\n";
			}

			unsigned int num_lh_samples = (unsigned int)block->lh_T.size();
			os << num_lh_samples << "\n";

			for( i=0; i < num_lh_samples; i++ )
			{
				float tx = block->lh_T[ i ].translation.x();
				float ty = block->lh_T[ i ].translation.y();
				float tz = block->lh_T[ i ].translation.z();

				float qw = block->lh_T[ i ].rotation.w();
				float qx = block->lh_T[ i ].rotation.x();
				float qy = block->lh_T[ i ].rotation.y();
				float qz = block->lh_T[ i ].rotation.z();

				os << tx << "\t" << ty << "\t" << tz << "\t" << qw << "\t" << qx << "\t" << qy << "\t" << qz << "\n";
			}

			unsigned int num_rh_samples = (unsigned int)block->rh_T.size();
			os << num_rh_samples << "\n";

			for( i=0; i < num_rh_samples; i++ )
			{
				float tx = block->rh_T[ i ].translation.x();
				float ty = block->rh_T[ i ].translation.y();
				float tz = block->rh_T[ i ].translation.z();

				float qw = block->rh_T[ i ].rotation.w();
				float qx = block->rh_T[ i ].rotation.x();
				float qy = block->rh_T[ i ].rotation.y();
				float qz = block->rh_T[ i ].rotation.z();

				os << tx << "\t" << ty << "\t" << tz << "\t" << qw << "\t" << qx << "\t" << qy << "\t" << qz << "\n";
			}

			unsigned int num_lf_samples = (unsigned int)block->lf_T.size();
			os << num_lf_samples << "\n";

			for( i=0; i < num_lf_samples; i++ )
			{
				float tx = block->lf_T[ i ].translation.x();
				float ty = block->lf_T[ i ].translation.y();
				float tz = block->lf_T[ i ].translation.z();

				float qw = block->lf_T[ i ].rotation.w();
				float qx = block->lf_T[ i ].rotation.x();
				float qy = block->lf_T[ i ].rotation.y();
				float qz = block->lf_T[ i ].rotation.z();

				os << tx << "\t" << ty << "\t" << tz << "\t" << qw << "\t" << qx << "\t" << qy << "\t" << qz << "\n";
			}

			unsigned int num_rf_samples = (unsigned int)block->rf_T.size();
			os << num_rf_samples << "\n";

			for( i=0; i < num_rf_samples; i++ )
			{
				float tx = block->rf_T[ i ].translation.x();
				float ty = block->rf_T[ i ].translation.y();
				float tz = block->rf_T[ i ].translation.z();

				float qw = block->rf_T[ i ].rotation.w();
				float qx = block->rf_T[ i ].rotation.x();
				float qy = block->rf_T[ i ].rotation.y();
				float qz = block->rf_T[ i ].rotation.z();

				os << tx << "\t" << ty << "\t" << tz << "\t" << qw << "\t" << qx << "\t" << qy << "\t" << qz << "\n";
			}
		}
		os << "\n";
	}
	return true;
}

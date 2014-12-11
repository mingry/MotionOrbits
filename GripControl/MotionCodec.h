#pragma once

#include "mathclass/transq.h"
#include "MotionCluster.h"

class SkeletalMotion;
class Joint;

struct CompressedBlock
{
	unsigned int original_f1;
	unsigned int original_fN;

	unsigned int compressed_f1;
	unsigned int compressed_fN;

	bool is_lossy;
	std::vector< math::transq > pelvis_T;
	std::vector< math::transq > lh_T;
	std::vector< math::transq > rh_T;
	std::vector< math::transq > lf_T;
	std::vector< math::transq > rf_T;
};

class MotionCodec
{
public:
	MotionCodec();
	virtual ~MotionCodec();

	bool encode( const std::string& path_original_motion, const std::string& path_cluster, unsigned int num_classes, unsigned int sampling_interval );
	bool decode( const std::string& path_compressed_motion, const std::string& path_block );
	void clear();

	bool saveEncodedMotion( const std::string& path_compressed_motion, const std::string& path_block );
	bool loadEncodedMotion( const std::string& path_compressed_motion, const std::string& path_block );

	inline SkeletalMotion* getOriginalMotion()		{ return original_motion; }
	inline SkeletalMotion* getCompressedMotion()	{ return compressed_motion; }
	inline SkeletalMotion* getDecompressedMotion()	{ return decompressed_motion; }

	inline void setPelvis( Joint* j )		{ pelvis = j; }
	inline void setLeftHand( Joint* j )		{ left_hand = j; }
	inline void setRightHand( Joint* j )	{ right_hand = j; }
	inline void setLeftFoot( Joint* j )		{ left_foot = j; }
	inline void setRightFoot( Joint* j )	{ right_foot = j; }

protected:
	SkeletalMotion* original_motion;
	std::vector< MotionCluster* > cluster_array;
	unsigned int original_num_frames;

	SkeletalMotion* compressed_motion;
	std::vector< CompressedBlock* > block_list;
	unsigned int compressed_num_frames;
	unsigned int sampling_interval;

	SkeletalMotion* decompressed_motion;

	Joint* pelvis;
	Joint* left_hand;
	Joint* right_hand;
	Joint* left_foot;
	Joint* right_foot;
};

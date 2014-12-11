#pragma once

#include "mathclass/position.h"
#include "mathclass/vector.h"
#include "mathclass/quater.h"
#include "mathclass/transq.h"

#include "SkeletalMotion.h"
#include "MotionGraph.h"

#include <list>
#include <map>
#include <deque>

class GraphSequence
{
public:
	struct Shot {
		MotionGraph::Node* node;
	};

	GraphSequence();
	virtual ~GraphSequence();

	void initialize( SkeletalMotion* m, MotionGraph* g );
	void finalize();
	void synthesize();

	Shot* insertNodeAtFront( MotionGraph::Node* node );
	Shot* insertNodeAtBack( MotionGraph::Node* node );
	Shot* insertNode( MotionGraph::Node* node, unsigned int i );

	MotionGraph::Node* removeNodeAtFront();
	MotionGraph::Node* removeNodeAtBack();
	MotionGraph::Node* removeNode( unsigned int i );

	std::deque< MotionGraph::Node* >* findTransition( Shot* prev_shot, Shot* next_shot );

	inline SkeletalMotion* getOriginalMotion()		{ return original_motion; }
	inline SkeletalMotion* getSynthesizedMotion()	{ return synthesized_motion; }
	inline MotionGraph* getMotionGraph()			{ return motion_graph; }

	inline void setBlendLength( unsigned int l )	{ blend_length = l; }
	inline unsigned int getBlendLength()			{ return blend_length; }

	inline std::vector< std::pair<unsigned int, unsigned int> >* getFrameIntervals()	{ return &frame_intervals; }

protected:
	bool addTransition( Shot* prev_shot, Shot* next_shot );
	bool removeTransition( Shot* prev_shot, Shot* next_shot );

	//
	SkeletalMotion* original_motion;
	SkeletalMotion*	synthesized_motion;
	MotionGraph*	motion_graph;

	unsigned int blend_length;

	std::list< Shot* > key_shots;
	std::map< std::pair<Shot*, Shot*>, std::deque< MotionGraph::Node* >* > transition_paths;
	std::vector< std::pair<unsigned int, unsigned int> >	frame_intervals;
};

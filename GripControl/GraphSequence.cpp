#include "GraphSequence.h"

#include "SkeletalMotion.h"
#include "MotionGraph.h"

#define DEFAULT_BLEND_LENGTH	4

GraphSequence::GraphSequence()
{
	original_motion = 0;
	synthesized_motion = 0;
	motion_graph = 0;

	blend_length = DEFAULT_BLEND_LENGTH;
}

GraphSequence::~GraphSequence()
{
	finalize();
}

void GraphSequence::initialize( SkeletalMotion* m, MotionGraph* g )
{
	finalize();

	original_motion = m;
	motion_graph = g;
}

void GraphSequence::finalize()
{
	original_motion = 0;
	motion_graph = 0;

	delete synthesized_motion;
	synthesized_motion = 0;

	std::list< Shot* >::iterator itor_s = key_shots.begin();
	while( itor_s != key_shots.end() )
	{
		Shot* shot = ( *itor_s ++ );
		delete shot;
	}
	key_shots.clear();

	std::map< std::pair<Shot*,Shot*>, std::deque<MotionGraph::Node*>* >::iterator itor_t = transition_paths.begin();
	while( itor_t != transition_paths.end() )
	{
		std::deque< MotionGraph::Node* >* path = itor_t->second;
		delete path;
		itor_t ++;
	}
	transition_paths.clear();
	frame_intervals.clear();
}

GraphSequence::Shot* GraphSequence::insertNode( MotionGraph::Node* node, unsigned int i )
{
	unsigned int num_shots = (unsigned int)key_shots.size();
	if( i==0 )				return insertNodeAtFront( node );
	if( i==num_shots )		return insertNodeAtBack( node );
	if( i > num_shots )		return 0;
	
	unsigned int index = 0;
	Shot* prev_shot = 0;

	std::list< Shot* >::iterator itor_s = key_shots.begin();
	while( itor_s != key_shots.end() )
	{
		Shot* next_shot = ( *itor_s );

		if( index == i )
		{
			Shot* shot = new Shot;
			shot->node = node;

			addTransition( prev_shot, shot );
			addTransition( shot, next_shot );

			key_shots.insert( itor_s, shot );

			return shot;
		}
		prev_shot = next_shot;
		itor_s ++;
	}
	return 0;
}

GraphSequence::Shot* GraphSequence::insertNodeAtFront( MotionGraph::Node* node )
{
	Shot* shot = new Shot;
	shot->node = node;

	if( !key_shots.empty() )
	{
		Shot* next_shot = key_shots.front();
		addTransition( shot, next_shot );
	}

	key_shots.push_front( shot );
	return shot;
}

GraphSequence::Shot* GraphSequence::insertNodeAtBack( MotionGraph::Node* node )
{
	Shot* shot = new Shot;
	shot->node = node;

	if( !key_shots.empty() )
	{
		Shot* prev_shot = key_shots.back();
		addTransition( prev_shot, shot );
	}

	key_shots.push_back( shot );
	return shot;
}

MotionGraph::Node* GraphSequence::removeNode( unsigned int i )
{
	unsigned int num_shots = (unsigned int)key_shots.size();
	if( i==0 )				return removeNodeAtFront();
	if( i==num_shots-1 )	return removeNodeAtBack();
	if( i >= num_shots )	return 0;

	unsigned int index = 0;
	Shot* prev_shot = 0;
	Shot* next_shot = 0;

	std::list< Shot* >::iterator itor_s = key_shots.begin();
	while( itor_s != key_shots.end() )
	{
		Shot* shot = *( itor_s );

		if( index == i )
		{
			itor_s ++;
			next_shot = *( itor_s );

			itor_s --;
			key_shots.erase( itor_s );

			removeTransition( prev_shot, shot );
			removeTransition( shot, next_shot );

			MotionGraph::Node* node = shot->node;
			delete shot;

			return node;
		}
		itor_s ++;
		index ++;

		prev_shot = shot;
	}
	return 0;
}

MotionGraph::Node* GraphSequence::removeNodeAtFront()
{
	Shot* shot = key_shots.front();
	key_shots.pop_front();

	Shot* next_shot = key_shots.front();
	removeTransition( shot, next_shot );

	MotionGraph::Node* node = shot->node;
	delete shot;

	return node;
}

MotionGraph::Node* GraphSequence::removeNodeAtBack()
{
	Shot* shot = key_shots.back();
	key_shots.pop_back();

	Shot* prev_shot = key_shots.back();
	removeTransition( prev_shot, shot );

	MotionGraph::Node* node = shot->node;
	delete shot;

	return node;
}

bool GraphSequence::addTransition( Shot* prev_shot, Shot* next_shot )
{
	std::deque< MotionGraph::Node* >* path = findTransition( prev_shot, next_shot );
	if( path )
	{
		return false;
	}

	MotionGraph::Node* start_node = prev_shot->node;
	MotionGraph::Node* end_node = next_shot->node;

	path = new std::deque< MotionGraph::Node* >;
	motion_graph->findPath( start_node, end_node, path );

	transition_paths[ std::make_pair(prev_shot, next_shot) ] = path;

	return true;
}

bool GraphSequence::removeTransition( Shot* prev_shot, Shot* next_shot )
{
	std::map< std::pair<Shot*,Shot*>, std::deque< MotionGraph::Node* >* >::iterator itor_p = transition_paths.find( std::make_pair(prev_shot, next_shot) );
	if( itor_p != transition_paths.end() )
	{
		std::deque< MotionGraph::Node* >* path = itor_p->second;
		delete path;

		transition_paths.erase( itor_p );
		return true;
	}
	else
	{
		return false;
	}
}

std::deque< MotionGraph::Node* >* GraphSequence::findTransition( Shot* prev_shot, Shot* next_shot )
{
	std::map< std::pair<Shot*,Shot*>, std::deque< MotionGraph::Node* >* >::iterator itor_p = transition_paths.find( std::make_pair(prev_shot, next_shot) );
	if( itor_p != transition_paths.end() )
	{
		std::deque< MotionGraph::Node* >* path = itor_p->second;
		return path;
	}
	else
	{
		return 0;
	}
}

void GraphSequence::synthesize()
{
	if( key_shots.empty() )
	{
		return;
	}

	delete synthesized_motion;
	frame_intervals.clear();
	
	// (1) shots --> nodes
	std::vector< MotionGraph::Node* > node_sequence;

	std::list< Shot* >::iterator itor_s = key_shots.begin();
	while( itor_s != key_shots.end() )
	{
		Shot* shot = ( *itor_s ++ );
		Shot* next_shot = ( itor_s != key_shots.end() ? *itor_s : 0 );

		MotionGraph::Node* start_node = shot->node;
		node_sequence.push_back( start_node );

		if( next_shot )
		{
			MotionGraph::Node* end_node = next_shot->node;

			std::deque< MotionGraph::Node* >* path = findTransition( shot, next_shot );

			unsigned int index = 0;
			unsigned int length = (unsigned int)path->size();

			std::deque< MotionGraph::Node* >::iterator itor_n = path->begin();
			while( itor_n != path->end() )
			{
				MotionGraph::Node* node = ( *itor_n ++ );

				if( index != 0 && index != length-1 )	// excluding start and end nodes
				{
					node_sequence.push_back( node );
				}

				index ++;
			}
		}
	}

	// (2) nodes --> segments
	unsigned int num_frames = 0;

	MotionGraph::Node* prev_node = 0;
	std::vector< std::pair<unsigned int, unsigned int> > segment_sequence;
	std::pair< unsigned int, unsigned int > prev_segment;

	std::vector< MotionGraph::Node* >::iterator itor_n = node_sequence.begin();
	while( itor_n != node_sequence.end() )
	{
		MotionGraph::Node* node = ( *itor_n ++ );
		std::vector< std::pair<unsigned int, unsigned int> >* segments = node->getSegments();
		std::pair< unsigned int, unsigned int > segment;
		
		if( prev_node )
		{
			unsigned int num_segments = (unsigned int)segments->size();
			unsigned int min_index = 0, i;
			double min_dist = DBL_MAX;

			for( i=0; i < num_segments; i++ )
			{
				std::pair< unsigned int, unsigned int > candidate = ( *segments )[ i ];
				double dist = SkeletalMotion::calcDistance( 
					original_motion, prev_segment.second,
					original_motion, candidate.first
				);
				if( dist < min_dist )
				{
					min_dist = dist;
					min_index = i;
				}
			}
			segment = ( *segments )[ min_index ];
		}
		else
		{
			segment = ( *segments )[ 0 ];
		}

		segment_sequence.push_back( segment );

		unsigned int segment_length = ( segment.second - segment.first + 1 );
		frame_intervals.push_back( std::make_pair( num_frames, num_frames+segment_length-1 ) );
		num_frames += segment_length;

		prev_node = node;
		prev_segment = segment;
	}

	// (3) segments --> motion
	synthesized_motion = new SkeletalMotion;
	synthesized_motion->initialize( original_motion->getSkeleton(), num_frames );

	unsigned int num_segments = (unsigned int)segment_sequence.size();
	unsigned int segment_index = 0;

	math::transq T = math::identity_transq;
	unsigned int prev_fN = 0;
	unsigned int num_synth_frames = 0;
	bool  is_prev_stitched = false;

	std::vector< std::pair<unsigned int, unsigned int> >::iterator itor_f = segment_sequence.begin();
	while( itor_f != segment_sequence.end() )
	{
		unsigned int f1 = itor_f->first;
		unsigned int fN = itor_f->second;
		unsigned int length = fN - f1 + 1;
		itor_f ++;

		unsigned int copy_f1 = f1;
		unsigned int copy_fN = fN;

		// decide T
		if( segment_index == 0 )
		{
			original_motion->transFromPlace( f1, &T, 0, 0, 0 );			
		}
		else
		{
			double x, z, angle;
			original_motion->placeFromTrans( prev_fN, T, &x, &z, &angle );
			original_motion->transFromPlace( f1, &T, x, z, angle );
		}

		// decide f1
		if( is_prev_stitched )
		{
			copy_f1 += blend_length;
		}

		// decide fN, and try to blend if necessary
		SkeletalMotion* stitching_motion = 0;

		if( segment_index < num_segments-1 )
		{
			unsigned int next_f1 = itor_f->first;
			unsigned int next_fN = itor_f->second;
			unsigned int next_length = next_fN - next_f1 + 1;

			double next_x, next_z, next_angle;
			math::transq next_T;

			original_motion->placeFromTrans( fN, T, &next_x, &next_z, &next_angle );
			original_motion->transFromPlace( next_f1, &next_T, next_x, next_z, next_angle );

			if( next_length > 2*blend_length )
			{
				stitching_motion = SkeletalMotion::stitch( original_motion, fN, T, original_motion, next_f1, next_T, blend_length );
				if( stitching_motion )
				{
					copy_fN -= blend_length;
				}
			}
		}

		// copy original motion
		synthesized_motion->copyFrom( original_motion, copy_f1, copy_fN, T, num_synth_frames );
		num_synth_frames += ( copy_fN-copy_f1+1 );

		// copy blend motion, if exist
		if( stitching_motion )
		{
			synthesized_motion->copyFrom( stitching_motion, 0, 2*blend_length-1, math::identity_transq, num_synth_frames );
			num_synth_frames += ( 2*blend_length );

			delete stitching_motion;
			is_prev_stitched = true;
		}
		else
		{
			is_prev_stitched = false;
		}

		//
		segment_index ++;
		prev_fN = fN;
	}
}

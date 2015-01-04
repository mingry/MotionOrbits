
#include "FLTKU/Animation.h"
#include "FLTKU/AnimationApp.h"

#include "DrawingTool.h"
#include "SkeletalMotion.h"
#include "MotionData.h"
#include "Skeleton.h"
#include "Joint.h"
#include "Human.h"
#include "PoseData.h"

#include "MotionGraph.h"
#include "OrbitGraph.h"
#include "Character.h"
#include "StickFigureSearch.h"




DrawingTool drawing_tool;


////////////// Motion and Graph
#define PATH_BVH	"../data/b-boy/B_boy.bvh"
//#define PATH_BVH	"../data/basketball/shooting.bvh"

#define PATH_GRAPH	"../data/b-boy/graph.txt"
#define PATH_ORBIT	"../data/b-boy/orbit.txt"


extern void setupBboySkeleton( Skeleton* s );
extern void setupBoxingSkeleton( Skeleton* s );
extern void setupCMUSkeleton( Skeleton* s );
extern void setupBasketballSkeleton( Skeleton* s );


class SketchControledAnimation : public mg::Animation
{

public:
	enum
	{
		CYCLING_IN_ORBIT = 0,
		LEAVING_FROM_ORBIT,
		ENTERING_INTO_ORBIT,
	};


	SketchControledAnimation()
	{
		motion_data_ = new SkeletalMotion;
		motion_graph_ = new MotionGraph;
		character_ = new Character;

		motion_data_->importFromBVH( PATH_BVH );
		motion_graph_->load( PATH_GRAPH );

		//setupBoxingSkeleton( motion_data.getSkeleton() );
		setupBboySkeleton( motion_data_->getSkeleton() );
		//setupCMU14Skeleton( motion_data.getSkeleton() );
		//setupBasketballSkeleton( motion_data.getSkeleton() );

		//

		MotionGraph::Node* start_node = motion_graph_->getNode(0);
		character_->embody( motion_data_, motion_graph_ );
		character_->extendPath( start_node );
		character_->place( 0, 0, 0 );

		target_node_ = 0;

		// InitializeStickFigureSearch( PATH_BVH, "../data/b-boy/B_boy_joint_map.txt");
		InitializeStickFigureSearch( "../data/b-boy/B_boy_s.bvh", "../data/b-boy/B_boy_joint_map.txt");
		AddStickFigureSearchCB( StaticStickFigureSearchListener, this );
	};

	void DrawCharacter()
	{
		float thickness = 5.0f;

		if( character_->isInBlend() )
		{
			SkeletalMotion* blend_motion = character_->getBlendMotion();
			math::transq T = character_->getTransform();
			unsigned int f = character_->getBlendFrame();

			drawing_tool.setColor( 0, 0.4, 1, 1 );
			drawing_tool.drawPose( blend_motion, f, thickness, T );
		}
		else
		{
			SkeletalMotion* skeletal_motion = character_->getSkeletalMotion();
			math::transq T = character_->getTransform();
			unsigned int f = character_->getFrame();

			drawing_tool.setColor( 1, 0.4, 0, 1 );
			drawing_tool.drawPose( skeletal_motion, f, thickness, T );
		}
	}

	virtual void FrameChanged(int frame)
	{
		mg::Animation::FrameChanged(frame);
		
		unsigned int node_path_len = character_->getNodePathLength();
		if( node_path_len <= 1 )
		{
			MotionGraph::Node* curr_node = character_->getPathNode( 0 );
			MotionGraph::Node* next_node = 0;

			if ( target_node_ == 0 )
			{
				next_node = curr_node->getNextEdges()->front()->getToNode();
			}
			else 
			{
				std::deque<MotionGraph::Node*> path;
				motion_graph_->findPath(curr_node, target_node_, &path);
				if ( path.size() > 0 )
				{
					next_node = path.front();

				}
				else
				{
					next_node = curr_node->getNextEdges()->front()->getToNode();
				}
			}

			character_->extendPath( next_node );
		}
		else
		{
			character_->update();
		}

	}

	/*virtual int CountFrames() const
	{
		return (int)num_frames_;
	}*/

	virtual void Draw(int frame)
	{

		DrawCharacter();
	}


	virtual int HandleFLTK(mg::AnimationViewer *w, int event) 
	{ 
		if ( event == FL_SHORTCUT && Fl::event_key() == '1')
		{
			return 1;
		}

		return mg::Animation::HandleFLTK(w, event); 
	}


	static void StaticStickFigureSearchListener(int start_frame, int frame_len, void *data)
	{
		((SketchControledAnimation*)data)->StickFigureSearchListener(start_frame, frame_len);
	}

	void StickFigureSearchListener(int searched_start_frame, int searched_frame_len)
	{
		if ( searched_start_frame < 0 || searched_frame_len <= 0 )
		{
			return;
		}

		target_node_ = 0;

		// We will check every node in motion_graph_ list
		for ( unsigned int i=0; i<motion_graph_->getNodeList()->size(); i++ )
		{
			MotionGraph::Node *node = motion_graph_->getNode(i);
			if ( searched_start_frame >= (int)node->getSegment(0).first
				&& searched_start_frame <= (int)node->getSegment(0).second )
			{
				target_node_ = node;
				break;
			}
		}

	}

protected:
	SkeletalMotion	*motion_data_;
	MotionGraph		*motion_graph_;
	Character		*character_;

	MotionGraph::Node *target_node_;
};


int main()
{

	mg::AnimationApp *app = new mg::AnimationApp;

	app->end();
	app->show();
	app->ani_frame_control_->play_mode(mg::AnimationFrameController::TO_INFINITE);

	SketchControledAnimation *ani = new SketchControledAnimation;
	ani->name("ani");
	app->ani_browser_->AddAnimation(ani);
	

	// InitializeStickFigureSearch("../data/b-boy/B_boy.bvh", "../data/b-boy/B_boy_joint_map.txt");
	
	return Fl::run();
}





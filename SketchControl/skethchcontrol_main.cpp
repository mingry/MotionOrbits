
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

static SkeletalMotion	g_motion_data;
static MotionGraph	g_motion_graph;
static OrbitGraph		g_orbit_graph( &g_motion_graph );

extern void setupBboySkeleton( Skeleton* s );
extern void setupBoxingSkeleton( Skeleton* s );
extern void setupCMUSkeleton( Skeleton* s );
extern void setupBasketballSkeleton( Skeleton* s );

void InitializeMotionGraph()
{
	g_motion_data.importFromBVH( PATH_BVH );
	g_motion_graph.load( PATH_GRAPH );
	g_orbit_graph.load( PATH_ORBIT );

	//setupBoxingSkeleton( motion_data.getSkeleton() );
	setupBboySkeleton( g_motion_data.getSkeleton() );
	//setupCMU14Skeleton( motion_data.getSkeleton() );
	//setupBasketballSkeleton( motion_data.getSkeleton() );


	
	
}


class CycleAnimation : public mg::Animation
{
public:
	CycleAnimation(OrbitGraph::Node *cycle)
	{
		// The input parameter cycle contains a list of a motion graph nodes, which is a sequence of cyclic motion. 
		nodes_in_cycle_ = cycle;

		// We need an instance of Character class, to synthesis a contineuous motion sequence. 
		// Becase Character class has the functions for traveling between nodes in a motion graph while connecting the postures in each node as a continues motion.
		Character character;
		character.embody( &g_motion_data, &g_motion_graph );
		
		// Set the nodes of the cyclic motion segemnt in the character_
		for ( unsigned int i=0; i<nodes_in_cycle_->getCycleSize(); i++ )
		{
			character.extendPath( (*nodes_in_cycle_->getCycle())[i] );
		}
		character.extendPath( nodes_in_cycle_->getCycle()->front() );

		// Calcule the number of frames of the cyclic motion
		num_frames_ = 0;
		for ( unsigned int i=0; i<character.getNodePathLength(); i++ )
		{
			if ( character.getPathNode(i)->getNumSegments() > 0 )
				if ( i==character.getNodePathLength()-1 )
					num_frames_ += 1;//character.getPathNode(i)->getSegment(0).second - character.getPathNode(i)->getSegment(0).first + 1;
				else
					num_frames_ += character.getPathNode(i)->getSegment(0).second - character.getPathNode(i)->getSegment(0).first + 1;
		}


		// Set the starting position as the origin of the given coordinate
		character.place( 0, 0, 0 );

		// The member variable, cycle_motion_, is an instance of MotionData class.
		// We will synthesis a cyclic motion data of 'cycle'
		cyclic_motion_.initialize(g_motion_data.getSkeleton(), num_frames_);

		for ( unsigned int i=0; i< num_frames_; i++ )
		{
			character.update();
			PoseData pose;
			pose.initialize( g_motion_data.getNumJoints() );

			if ( character.isInBlend() )
			{
				pose.copy( character.getBlendMotion()->getPoseData( character.getBlendFrame() ) );
			}
			else
			{
				pose.copy( character.getSkeletalMotion()->getPoseData( character.getFrame() ) );
			}
			pose.transform( character.getTransform() );

			cyclic_motion_.setPoseData(i, &pose);
		}


	};

	void DrawCharacter()
	{
		float thickness = 5.0f;

		drawing_tool.setColor( 1, 0.4, 0, 1 );
		drawing_tool.drawPose( &cyclic_motion_, this->cur_frame(), thickness);
	}

	virtual void FrameChanged(int frame)
	{
		mg::Animation::FrameChanged(frame);
	}

	virtual int CountFrames() const
	{
		return (int)num_frames_;
	}

	virtual void Draw(int frame)
	{

		if ( nodes_in_cycle_ == 0 ) return;

		DrawCharacter();
	}

private:
	SkeletalMotion cyclic_motion_;
	
	OrbitGraph::Node *nodes_in_cycle_;
	int num_frames_;
};




int main()
{
	InitializeMotionGraph();
	InitializeStickFigureSearch("data/b-boy/B_boy.bvh", "../data/b-boy/B_boy_joint_map.txt");

	mg::AnimationApp *app = new mg::AnimationApp;

	app->end();
	app->show();




	for ( int i=0; i<g_orbit_graph.getNumNodes(); i++ )
	{
		CycleAnimation *ani = new CycleAnimation(g_orbit_graph.getNode(i));
		char ani_name[256];
		sprintf_s(ani_name, "Cycle_%d", i);
		ani->name(ani_name);

		app->ani_browser_->AddAnimation(ani);
	}

	
	return Fl::run();
}
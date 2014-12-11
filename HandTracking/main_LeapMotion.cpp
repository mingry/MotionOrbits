#define NOMINMAX

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <iterator>

#include <GL/glew.h>
#include <GL/glut.h>

#include "DrawingTool.h"
#include "trackball.h"
#include "CameraDirector.h"
#include "Panel.h"

#include "SkeletalMotion.h"
#include "MotionData.h"
#include "Skeleton.h"
#include "Joint.h"
#include "Human.h"

#include "mathclass\matrixN.h"
#include "mathclass\vectorN.h"


#include "Leap.h"
using namespace Leap;


#include <seqan/find.h> 
#include <seqan/index.h>
using namespace seqan;


CRITICAL_SECTION cs;


#define PATH_BVH			"data/basketball/shooting.bvh"
#define PATH_STROKE			"data/basketball/%s.txt"
#define PATH_CLUSTER		"data/basketball/%s.txt"
#define PATH_STRING			"data/basketball/%s.txt"
#define PATH_TEST_RESULT	"data/basketball/test_result.txt"

/*
#define PATH_BVH		"data/boxing/boxing_shadow_m_edit.bvh"
#define PATH_STROKE		"data/boxing/%s.txt"
#define PATH_CLUSTER	"data/boxing/%s.txt"
*/

//#define PATH_BVH	"data/boxing/boxing_shadow_m_edit.bvh"
//#define PATH_BVH	"data/b-boy/B_boy.bvh"
//#define PATH_BVH	"data/CMU/14_13.bvh"

#define NUM_CAPTURED_POINTS		100
#define NUM_ICP					5
#define MIN_STROKE_LENGTH		5

//
static void initLeap();
static void initGLUT( int* argc, char** argv );
static void initGL();
static void initStrokeDB();

static void run();
static void finalize();

// glut callback functions
static void reshape( int w, int h );
static void display();
static void idle();
static void finalize();
static void timer( int timer_id );

static void keyboard( unsigned char key, int x, int y );
static void special( int key, int x, int y );
static void mouse( int button, int state, int x, int y );
static void motion( int x, int y );

//
static unsigned int win_width = 1500;
static unsigned int win_height = 750;
static float win_aspect_ratio = ( (float)win_width / (float)win_height );
static float fovy = 60.0f;
static float z_near = 1.0f;
static float z_far = 10000.0f;
static float view_height = 400.0;
static float view_width = 400.0;
static float view_distance = 400.0f;

//
enum {
	PERSPECTIVE_VIEW = 0,
	TOP_VIEW
};
		
static unsigned int view_mode = PERSPECTIVE_VIEW;

//
static bool is_lbutton_down = false;
static bool is_rbutton_down = false;
static int track_x, track_y;

static unsigned int current_frame = 0;
static unsigned int time_interval = 1;
static bool is_playing = true;
static bool is_simplified = false;

//
static CameraDirector	camera_director;
static QTrackBall		track_ball;
static SkeletalMotion	skeletal_motion;
extern DrawingTool		drawing_tool;

//
extern void setupBboySkeleton( Skeleton* s );
extern void setupBoxingSkeleton( Skeleton* s );
extern void setupCMUSkeleton( Skeleton* s );
extern void setupBasketballSkeleton( Skeleton* s );

//
class StrokeListener;
class Stroke;
class StrokeCluster;

enum SearchMode
{
	STROKE_QUERY = 0,
	MOTION_RETRIEVAL,
	VIEW_QUERY,
	VIEW_RESULT,
};

static StrokeListener* listener = 0;
static Controller* controller = 0;

static std::deque< math::position > captured_points;
static std::vector< math::position > stroke_points;

static bool was_recording = false;
static int search_mode = STROKE_QUERY;

static Panel	motion_panel;
static Panel	result_panel;

static void inputStroke();
static void removeStrokes();

//
class Stroke
{
public:
	Stroke()				{ f1 = fN = 0; }
	Stroke( int f )			{ f1 = fN = f; }
	Stroke( int a, int b )	{ f1 = a; fN = b; }

	int f1, fN;
	std::vector< math::position > pts;
};

class StrokeCluster
{
public:
	StrokeCluster( int id ) { this->id = id; }

	int id;
	Stroke* centroid;
	std::vector< Stroke* > members;
};

class QueryStroke
{
public:
	QueryStroke()	{}

	std::vector< Stroke* > initial_segments;
	std::vector< Stroke* > query_segments;
	std::vector< Stroke* > fitted_segments;
};

class RetrievedMotion
{
public:
	RetrievedMotion()	{}

	std::vector< StrokeCluster* > retrieved_clusters;
	std::vector< Stroke* > retrieved_segments;
};

static std::vector< Stroke* > lh_strokes;
static std::vector< Stroke* > rh_strokes;
static std::vector< Stroke* > lf_strokes;
static std::vector< Stroke* > rf_strokes;
static std::vector< Stroke* > hip_strokes;

static std::vector< StrokeCluster* > lh_clusters;
static std::vector< StrokeCluster* > rh_clusters;
static std::vector< StrokeCluster* > lf_clusters;
static std::vector< StrokeCluster* > rf_clusters;
static std::vector< StrokeCluster* > hip_clusters;

static std::vector< int > lh_string;
static std::vector< int > rh_string;
static std::vector< int > lf_string;
static std::vector< int > rf_string;
static std::vector< int > hip_string;

//
static std::vector< QueryStroke* >		query_list;
static std::vector< RetrievedMotion* >	retrieval_list;

//
static int type_index = 0;
static int cluster_index = 0;
static int segment_index = 0;
static StrokeCluster* problem_cluster = 0;
static Stroke* problem_segment = 0;

static int problem_f1;
static int problem_fN;
const static int problem_len = 60;

class UserTest
{
public:
	UserTest() 
	{ 
		f1 = fN = 0;
		num_motion_frames = 0;
		motion_length = 0;

		num_sketch_frames = 0;
		sketch_length = 0;

		num_retrieved_segments = 0;
		num_exact_segments = 0;
		num_apprx_segments = 0;
	}
	~UserTest() {}

	// problem
	int f1;
	int fN;
	int num_motion_frames;
	double motion_length;

	// answer
	int num_sketch_frames;
	double sketch_length;

	std::vector< math::position > sketch_points;

	// evaluation
	int num_retrieved_segments;
	int num_exact_segments;
	int num_apprx_segments;

	std::vector< std::pair<int, int> > exact_segments;
	std::vector< std::pair<int, int> > apprx_segments;

//	std::vector< int > cluster_indexes_for_exact_segments;
//	std::vector< int > cluster_indexes_for_apprx_segments;
};

std::vector< UserTest* > test_results;

static void setNewProblem()
{
	int num_frames = skeletal_motion.getNumFrames();

	problem_f1 = rand() % ( num_frames - problem_len - 1 );
	problem_fN = problem_f1 + problem_len;

	motion_panel.setLeft( win_width/2 );
	motion_panel.setTop( win_height );
	motion_panel.setWidth( win_width/2 );
	motion_panel.setHeight( win_height );

	motion_panel.setSkeletalMotion( &skeletal_motion );
	motion_panel.setStartFrame( problem_f1 );
	motion_panel.setEndFrame( problem_fN );
	motion_panel.setCurrentFrame( problem_f1 );
	
	camera_director.findBestConfiguration( &motion_panel );

	Joint* pelvis = skeletal_motion.getHumanJoint( Human::PELVIS );
	int pelvis_index = pelvis->getIndex();

	double len = 0;
	for( int f=problem_f1; f < problem_fN-1; f++ )
	{
		math::transq T0 = skeletal_motion.getGlobalTransform( f, pelvis_index );
		math::transq T1 = skeletal_motion.getGlobalTransform( f+1, pelvis_index );

		math::vector v( T1.translation - T0.translation );
		len += v.length();
	}

	//
	UserTest* user_test = new UserTest;
	test_results.push_back( user_test );

	user_test->f1 = problem_f1;
	user_test->fN = problem_fN;

	user_test->num_motion_frames = problem_fN - problem_f1 + 1;
	user_test->motion_length = len;
}

//
static bool loadString( const std::string& name, std::vector< int >* string );
static bool loadClusters( const std::string& name, std::vector< StrokeCluster* >* clusters, std::vector< Stroke* >* strokes );
static void iterative_closest_point( Stroke* stroke1, Stroke* stroke2, int num_iter );
static double calcStrokeDistance( Stroke* stroke1, Stroke* stroke2, int num_iter );
static void removeClusters();

inline static bool sort_eigen_vectors( std::pair<math::vector,double>& lhs, std::pair<math::vector,double>& rhs )
{
	return ( lhs.second > rhs.second );
}

static math::vector calcPrincipalDirection( Stroke* stroke )
{
	int num_pts = (int)stroke->pts.size(), j;

	math::matrixN mat;
	mat.setSize( num_pts, 3 );

	math::vectorN mean;
	mean.setSize( 3 );
	mean.setValue( 0, 0 );
	mean.setValue( 1, 0 );
	mean.setValue( 2, 0 );

	for( int j=0; j < num_pts; j++ )
	{
		math::position p = stroke->pts[j];

		math::vectorN v;
		v.setSize( 3 );
		v.setValue( 0, p.x() );
		v.setValue( 1, p.y() );
		v.setValue( 2, p.z() );
			
		mat.setRow( j, v );
		mean.add( mean, v );
	}
	mean.div( mean, (double)num_pts );

	for( j=0; j < num_pts; j++ )
	{
		mat.setValue( j, 0, mat.getValue(j,0)-mean[0] );
		mat.setValue( j, 1, mat.getValue(j,1)-mean[1] );
		mat.setValue( j, 2, mat.getValue(j,2)-mean[2] );
	}

	math::vectorN e;
	math::matrixN B;
	e.setSize( 3 );
	B.setSize( 3, 3 );
	
	mat.SVdecompose( e, B );

	math::vector v0( B.getValue(0,0), B.getValue(1,0), B.getValue(2,0) );
	math::vector v1( B.getValue(0,1), B.getValue(1,1), B.getValue(2,1) );
	math::vector v2( B.getValue(0,2), B.getValue(1,2), B.getValue(2,2) );

	v0 /= v0.length();
	v1 /= v1.length();
	v2 /= v2.length();

	std::vector< std::pair<math::vector, double> > eigen_vectors;
	eigen_vectors.push_back( std::make_pair(v0, e[0]) );
	eigen_vectors.push_back( std::make_pair(v1, e[1]) );
	eigen_vectors.push_back( std::make_pair(v2, e[2]) );

	std::sort( eigen_vectors.begin(), eigen_vectors.end(), sort_eigen_vectors );
	v0 = eigen_vectors[0].first;
	v1 = eigen_vectors[1].first;
	v2 = eigen_vectors[2].first;

	return v0;
}

static double calcPrincipalLength( Stroke* stroke )
{
	math::vector v = calcPrincipalDirection( stroke );
	v /= v.length();

	double min = DBL_MAX, max = -DBL_MAX;

	int num_pts = (int)stroke->pts.size(), j;
	for( j=0; j < num_pts; j++ )
	{
		math::position pj = stroke->pts[ j ];
		math::vector vj( pj.x(), pj.y(), pj.z() );

		double proj = vj % v;
		if( proj > max )	max = proj;
		if( proj < min )	min = proj;
	}

	double len = max - min;
	return len;
}

static bool loadString( const std::string& name, std::vector< int >* string )
{
	char path[128];
	sprintf( path, PATH_STRING, name.c_str() );
			
	std::ifstream is( path );
	if( !is.is_open() )	return false;

	string->clear();

	int num_letters = 0, l; 
	is >> num_letters;

	for( l=0; l < num_letters; l++ )
	{
		int index = 0;
		is >> index;

		string->push_back( index );
	}

	return true;
}

static bool loadClusters( const std::string& name, std::vector< StrokeCluster* >* clusters, std::vector< Stroke* >* strokes )
{
	char path[128];
	sprintf( path, PATH_CLUSTER, name.c_str() );
			
	std::ifstream is( path );
	if( !is.is_open() )	return false;

	int num_clusters = 0, c; 
	is >> num_clusters;

	for( c=0; c < num_clusters; c++ )
	{
		StrokeCluster* cluster = new StrokeCluster( c );
		clusters->push_back( cluster );

		int f1, fN, f, num_frames;
		double x, y, z;

		// centroid
		Stroke* centroid = new Stroke;
		strokes->push_back( centroid );
		cluster->centroid = centroid;

		is >> f1 >> fN;
		num_frames = fN - f1 + 1;

		centroid->f1 =f1;
		centroid->fN = fN;

		for( f=0; f < num_frames; f++ ) 
		{
			is >> x >> y >> z;
			centroid->pts.push_back( math::position(x,y,z) );
		}

		// members (including centroid again)
		int num_members = 0, m;
		is >> num_members;

		for( m=0; m < num_members; m++ )
		{
			is >> f1 >> fN;
			num_frames = fN - f1 + 1;
			
			Stroke* member = 0;
			if( f1 == centroid->f1 && fN == centroid->fN )	
			{
				member = centroid;
			}
			else
			{
				member = new Stroke;
				member->f1 = f1;
				member->fN = fN;

				strokes->push_back( member );
			}

			for( f=0; f < num_frames; f++ )
			{
				is >> x >> y >> z;
				if( member != centroid )
				{
					member->pts.push_back( math::position(x,y,z) );
				}
			}

			cluster->members.push_back( member );

//			iterative_closest_point( centroid, member, NUM_ICP );
		}
	}
	return true;
}

static void removeStrokes( std::vector< Stroke* >* strokes )
{
	std::vector< Stroke* >::iterator itor_s = strokes->begin();
	while( itor_s != strokes->end() )
	{
		Stroke* stroke = ( *itor_s ++ );
		delete stroke;
	}
	strokes->clear();
}

static void removeClusters( std::vector< StrokeCluster* >* clusters )
{
	std::vector< StrokeCluster* >::iterator itor_c = clusters->begin();
	while( itor_c != clusters->end() )
	{
		StrokeCluster* cluster = ( *itor_c ++ );
		delete cluster;
	}
	clusters->clear();
}

// Query-related per-session data
static void removeStrokes()
{
	std::vector< QueryStroke* >::iterator itor_q = query_list.begin();
	while( itor_q != query_list.end() )
	{
		QueryStroke* query = ( *itor_q ++ );

		removeStrokes( &query->initial_segments );
		removeStrokes( &query->query_segments );
		removeStrokes( &query->fitted_segments );

		delete query;
	}
	query_list.clear();

	std::vector< RetrievedMotion* >::iterator itor_r = retrieval_list.begin();
	while( itor_r != retrieval_list.end() )
	{
		RetrievedMotion* retrieval = ( *itor_r ++ );

		retrieval->retrieved_clusters.clear();
		retrieval->retrieved_segments.clear();

		delete retrieval;
	}
	retrieval_list.clear();
}

// Persistent database
static void removeClusters()
{
	removeStrokes( &rh_strokes );
	removeStrokes( &lh_strokes );
	removeStrokes( &rf_strokes );
	removeStrokes( &lf_strokes );
	removeStrokes( &hip_strokes );

	removeClusters( &rh_clusters );
	removeClusters( &lh_clusters );
	removeClusters( &rf_clusters );
	removeClusters( &lf_clusters );
	removeClusters( &hip_clusters );
}

// fit stroke2 to stroke1
static void iterative_closest_point( Stroke* stroke1, Stroke* stroke2, int num_iter )
{
	int iter, i, j;

	int len1 = stroke1->pts.size();
	int len2 = stroke2->pts.size();

	std::vector< math::position > *pts1, *pts2;
	pts1 = &stroke1->pts;
	pts2 = &stroke2->pts;

	len1 = pts1->size();
	len2 = pts2->size();

	for( iter=0; iter < num_iter; iter++ )
	{
		// 1. finding correspondences
		std::vector< std::pair<int, int> > corr;

		if( iter==0 )
		{
			for( j=0; j < len2; j++ )
			{
				double t = (double)j / (double)( len2-1 );
				int i = ( t * (double)( len1-1 ) + 0.5 );
				
				corr.push_back( std::make_pair(i, j) );
			}
		}
		else
		{
			for( i=0; i < len1; i++ ) 
			{
				math::position p1 = ( *pts1 )[ i ];

				double min_dist = +DBL_MAX;
				int min_j = 0;

				for( j=0; j < len2; j++ )
				{
					math::position p2 = ( *pts2 )[ j ];

					double dist = math::vector( p2-p1 ).length();
					if( dist < min_dist )
					{
						min_dist = dist;
						min_j = j;
					}
				}
				corr.push_back( std::make_pair(i, min_j) );
			}
		}

		// 2. rigid transformation
		int len = corr.size();
		double xzp = 0, xpz = 0, xxp = 0, zzp = 0;
		double xbar = 0, ybar = 0, zbar = 0;
		double xpbar = 0, ypbar = 0, zpbar = 0;

		for( i=0; i < len; i++ )
		{
			math::position p1 = ( *pts1 )[ corr[ i ].first ];
			math::position p2 = ( *pts2 )[ corr[ i ].second ];

			double x = p1.x();
			double y = p1.y();
			double z = p1.z();

			double xp = p2.x();
			double yp = p2.y();
			double zp = p2.z();

			xzp += ( x * zp );
			xpz += ( xp * z );
			xxp += ( x * xp );
			zzp += ( z * zp );

			xbar += x;
			ybar += y;
			zbar += z;

			xpbar += xp;
			ypbar += yp;
			zpbar += zp;
		}

		double X = ( xxp + zzp ) - ( xbar*xpbar + zbar*zpbar ) / len;
		double Z = ( xzp - xpz ) - ( xbar*zpbar - xpbar*zbar ) / len;
		double theta = atan2( Z, X );

//		std::cout << "Rotation: " << theta * 180.0 / 3.141592 << "( " << X << ", " << Z << ")\n";

		double X0 = ( xbar - xpbar*cos(theta) - zpbar*sin(theta) ) / len;
		double Y0 = ( ybar - ypbar ) / len;
		double Z0 = ( zbar + xpbar*sin(theta) - zpbar*cos(theta) ) / len;

		for( j=0; j < len2; j++ )
		{
			math::position p2 = ( *pts2 )[ j ];

			double xp = p2.x();
			double yp = p2.y();
			double zp = p2.z();

			double xpp = xp*cos(theta) + zp*sin(theta) + X0;
			double ypp = yp + Y0;
			double zpp = -xp*sin(theta) + zp*cos(theta) + Z0;

			( *pts2 )[ j ].set_x( xpp );
			( *pts2 )[ j ].set_y( ypp );
			( *pts2 )[ j ].set_z( zpp );
		}
	}
}

static double calcStrokeDistance( Stroke* stroke1, Stroke* stroke2, int num_iter )
{
	int iter, i, j;

	std::vector< math::position > pts1, pts2;
	int len1 = stroke1->pts.size();
	int len2 = stroke2->pts.size();
	
	if( len1 < len2 )
	{
		std::copy( stroke1->pts.begin(), stroke1->pts.end(), std::inserter( pts1, pts1.begin() ) );
		std::copy( stroke2->pts.begin(), stroke2->pts.end(), std::inserter( pts2, pts2.begin() ) );
	}
	else
	{
		std::copy( stroke1->pts.begin(), stroke1->pts.end(), std::inserter( pts2, pts2.begin() ) );
		std::copy( stroke2->pts.begin(), stroke2->pts.end(), std::inserter( pts1, pts1.begin() ) );
	}

	len1 = pts1.size();
	len2 = pts2.size();

	for( iter=0; iter < num_iter; iter++ )
	{
		// 1. finding correspondences (the nearest pair)
		std::vector< std::pair<int, int> > corr;
		{
			// DTW
			int n1 = len1;
			int n2 = len2;

			double* distance_array = new double[ (n1+1)*(n2+1) ];
			char* direction_array = new char[ (n1+1)*(n2+1) ];

		#define DTW(i,j)	distance_array[ (j)*(n1+1) + (i) ]
		#define DIR(i,j)	direction_array[ (j)*(n1+1) + (i) ]
		#define MIN3(i,j,k)	( (i) > (j) ? ( (j) > (k) ? (k) : (j) ) : ( (i) > (k) ? (k) : (i) ) )

			for( i=0; i <= n1; i++ )
			{
				DTW(i,0) = DBL_MAX;
			}

			for( j=0; j <= n2; j++ )
			{
				DTW(0,j) = DBL_MAX;
			}
			DTW(0,0) = 0;

			for( i=1; i <= n1; i++ )
			{
				for( j=1; j <= n2; j++ )
				{
					math::position p1 = pts1[ i-1 ];
					math::position p2 = pts2[ j-1 ];
					double cost = math::vector( p2-p1 ).length();
					double min = MIN3( DTW(i-1,j), DTW(i,j-1), DTW(i-1,j-1) );
			
					DTW(i,j) = cost + min;
					if( min == DTW(i-1,j) )	DIR(i,j) = 'i';
					if( min == DTW(i,j-1) ) DIR(i,j) = 'j';
					if( min == DTW(i-1,j-1) ) DIR(i,j) = 'k';
				}
			}

			i = n1;
			j = n2;

			unsigned int n = 1;
			while( !( i == 1 && j == 1 ) )
			{
				corr.push_back( std::make_pair(i-1, j-1) );

				if( DIR(i,j) == 'i' )		i--;
				else if( DIR(i,j) == 'j' )	j--;
				else	{ i--; j--; }
				
				n ++;
			}

			double dist = DTW(n1, n2) / (double)n;

			delete[] distance_array;
			delete[] direction_array;

		#undef MIN3
		#undef DTW
		}

		// 2. rigid transformation
		int len = corr.size();
		double xzp = 0, xpz = 0, xxp = 0, zzp = 0;
		double xbar = 0, zbar = 0, xpbar = 0, zpbar = 0;
		double ybar = 0, ypbar = 0;

		for( i=0; i < len; i++ )
		{
			math::position p1 = pts1[ corr[ i ].first ];
			math::position p2 = pts2[ corr[ i ].second ];

			double x = p1.x();
			double y = p1.y();
			double z = p1.z();

			double xp = p2.x();
			double yp = p2.y();
			double zp = p2.z();

			xzp += ( x * zp );
			xpz += ( xp * z );
			xxp += ( x * xp );
			zzp += ( z * zp );

			xbar += x;
			zbar += z;
			xpbar += xp;
			zpbar += zp;

			ybar += y;
			ypbar += yp;
		}

		double X = ( xxp + zzp ) - ( xbar*xpbar + zbar*zpbar ) / len;
		double Z = ( xzp - xpz ) - ( xbar*zpbar - xpbar*zbar ) / len;
		double theta = atan2( Z, X );

		double X0 = ( xbar - xpbar*cos(theta) - zpbar*sin(theta) ) / len;
		double Y0 = ( ybar - ypbar ) / len;
		double Z0 = ( zbar + xpbar*sin(theta) - zpbar*cos(theta) ) / len;

		for( j=0; j < len2; j++ )
		{
			math::position p2 = pts2[ j ];

			double xp = p2.x();
			double yp = p2.y();
			double zp = p2.z();

			double xpp = xp*cos(theta) + zp*sin(theta) + X0;
			double ypp = yp + Y0;
			double zpp = -xp*sin(theta) + zp*cos(theta) + Z0;

			pts2[ j ].set_x( xpp );
			pts2[ j ].set_y( ypp );
			pts2[ j ].set_z( zpp );
		}
	}

	// DTW
	int n1 = len1;
	int n2 = len2;

	double* distance_array = new double[ (n1+1)*(n2+1) ];
	char* direction_array = new char[ (n1+1)*(n2+1) ];

#define DTW(i,j)	distance_array[ (j)*(n1+1) + (i) ]
#define DIR(i,j)	direction_array[ (j)*(n1+1) + (i) ]
#define MIN3(i,j,k)	( (i) > (j) ? ( (j) > (k) ? (k) : (j) ) : ( (i) > (k) ? (k) : (i) ) )

	for( i=0; i <= n1; i++ )
	{
		DTW(i,0) = DBL_MAX;
	}

	for( j=0; j <= n2; j++ )
	{
		DTW(0,j) = DBL_MAX;
	}
	DTW(0,0) = 0;

	for( i=1; i <= n1; i++ )
	{
		for( j=1; j <= n2; j++ )
		{
			math::position p1 = pts1[ i-1 ];
			math::position p2 = pts2[ j-1 ];
			double cost = math::vector( p2-p1 ).length();
			double min = MIN3( DTW(i-1,j), DTW(i,j-1), DTW(i-1,j-1) );
			
			DTW(i,j) = cost + min;
			if( min == DTW(i-1,j) )	DIR(i,j) = 'i';
			if( min == DTW(i,j-1) ) DIR(i,j) = 'j';
			if( min == DTW(i-1,j-1) ) DIR(i,j) = 'k';
		}
	}

	i = n1;
	j = n2;

	unsigned int n = 1;
	while( !( i == 1 && j == 1 ) )
	{
		if( DIR(i,j) == 'i' )		i--;
		else if( DIR(i,j) == 'j' )	j--;
		else	{ i--; j--; }
		n ++;
	}

	double dist = DTW(n1, n2) / (double)n;

	delete[] distance_array;
	delete[] direction_array;

	return dist;

#undef MIN3
#undef DTW
}

static void findNearestClusters( 
	std::vector< math::position >* pts, std::vector< StrokeCluster* >* cluster_list, 
	std::vector< Stroke* >* initial_segments, std::vector< Stroke* >* query_segments, 
	std::vector< StrokeCluster* >* retrieved_clusters 
)
{
	int num_pts = pts->size();
	if( num_pts < MIN_STROKE_LENGTH )
	{
		return;
	}

	// (1) initial segmentation
	int prev_cut = 0;

	for( int i=2; i < num_pts-1; i++ )
	{
		math::vector prev_v( (*pts)[i-1] - (*pts)[i-2] );
		math::vector curr_v( (*pts)[i] - (*pts)[i-1] );
		math::vector next_v( (*pts)[i+1] - (*pts)[i] );

		double prev_s = prev_v.length();
		double curr_s = curr_v.length();
		double next_s = next_v.length();

		double dot = prev_v % next_v;

		if( prev_s < curr_s && next_s < curr_s ||
			prev_s > curr_s && next_s > curr_s )
		{
			Stroke* stroke = new Stroke( prev_cut, i );
			initial_segments->push_back( stroke );

			prev_cut = i;
		}
	}
	Stroke* stroke = new Stroke( prev_cut, num_pts-1 );
	initial_segments->push_back( stroke );

	// (2) process segments
	std::vector< Stroke* >::iterator itor_s = initial_segments->begin();
	while( itor_s != initial_segments->end() )
	{
		Stroke* stroke = ( *itor_s );

		int f1 = stroke->f1;
		int fN = stroke->fN;

		if( fN-f1+1 < MIN_STROKE_LENGTH )
		{
			delete stroke;
			itor_s = initial_segments->erase( itor_s );
		}
		else
		{
			for( int f=f1; f <= fN; f++ )
			{
				stroke->pts.push_back( stroke_points[ f ] );
			}
			itor_s ++;
		}
	}

	// (3) cluster-wise retrieval (incremental merging of adjacent segments)
	int si = 0, sj = 0, num_segments = initial_segments->size();

	while( si < num_segments )
	{
		sj = si;
		
		double old_dist = 0, new_dist = DBL_MAX;
		StrokeCluster *old_cluster = 0, *new_cluster = 0;

		do
		{
			old_dist = new_dist;
			old_cluster = new_cluster;

			int f1 = ( *initial_segments )[si]->f1;
			int fN = ( *initial_segments )[sj]->fN;

			Stroke* new_stroke = new Stroke( f1, fN );
			for( int f=f1; f <= fN; f++ )
			{
				math::position p = stroke_points[ f ];
				new_stroke->pts.push_back( p );
			}	
			double length = calcPrincipalLength( new_stroke );

			//
			double min_dist = DBL_MAX;
			StrokeCluster* min_cluster = 0;

			std::vector< StrokeCluster* >::iterator itor_c = cluster_list->begin();
			while( itor_c != cluster_list->end() )
			{
				StrokeCluster* cluster = ( *itor_c ++ );
				Stroke* center = cluster->centroid;

				double Length = calcPrincipalLength( center );
				double scale = Length / length;

				for( int f = f1; f <= fN; f++ )
				{
					math::position p = new_stroke->pts[ f-f1 ];
					p.set_x( p.x() * scale );
					p.set_y( p.y() * scale );
					p.set_z( p.z() * scale );

					new_stroke->pts[ f-f1 ] = p;
				}

				double dist = calcStrokeDistance( center, new_stroke, NUM_ICP );
//				dist /= Length;
				dist /= scale;

				if( dist < min_dist )
				{
					min_cluster = cluster;
					min_dist = dist;
				}
			}
			new_dist = min_dist;
			new_cluster = min_cluster;

			//
			delete new_stroke;
			sj ++;
		}
		while( new_dist < old_dist && sj < num_segments );

		sj --;

		int f1 = ( *initial_segments )[si]->f1;
		int fN = ( *initial_segments )[sj]->fN;

		Stroke* query_stroke = new Stroke( f1, fN );
		for( int f=f1; f <= fN; f++ )
		{
			query_stroke->pts.push_back( stroke_points[ f ] );
		}
		query_segments->push_back( query_stroke );
		retrieved_clusters->push_back( old_cluster? old_cluster : new_cluster );

		si = sj + 1;
	}
}

static double findNearestSegments
( 
	std::vector< Stroke* >* query_segments, std::vector< StrokeCluster* >* retrieved_clusters, 
	std::vector< Stroke* >* retrieved_segments, std::vector< Stroke* >* fitted_segments 
)
{
	double sum_dist = 0;
	int num_queries = (int)query_segments->size();
	for( int q=0; q < num_queries; q++ )
	{
		Stroke* query_stroke = ( *query_segments )[ q ];
		StrokeCluster* retrieved_cluster = ( *retrieved_clusters )[ q ];
		
		std::vector< Stroke* >* cluster_strokes = &retrieved_cluster->members;
		int num_cluster_strokes = (int)cluster_strokes->size();

		double min_dist = DBL_MAX;
		Stroke* min_stroke = 0;

		for( int s=0; s < num_cluster_strokes; s++ )
		{
			Stroke* cluster_stroke = ( *cluster_strokes )[ s ];

			double len = calcPrincipalLength( query_stroke );
			double Len = calcPrincipalLength( cluster_stroke );
			double scale = Len / len;

			int f1 = query_stroke->f1;
			int fN = query_stroke->fN;
			Stroke* new_query_stroke = new Stroke( f1, fN );

			for( int f = f1; f <= fN; f++ )
			{
				math::position p = query_stroke->pts[ f-f1 ];
				p.set_x( p.x() * scale );
				p.set_y( p.y() * scale );
				p.set_z( p.z() * scale );

				new_query_stroke->pts.push_back( p );
			}

			double dist = calcStrokeDistance( cluster_stroke, new_query_stroke, NUM_ICP );
			dist /= scale;

			if( dist < min_dist )
			{
				min_dist = dist;
				min_stroke = cluster_stroke;
			}
			delete new_query_stroke;
		}
		retrieved_segments->push_back( min_stroke );
		sum_dist += min_dist;

		// fit to motion space
		double len = calcPrincipalLength( query_stroke );
		double Len = calcPrincipalLength( min_stroke );
		double scale = Len / len;

		int f1 = query_stroke->f1;
		int fN = query_stroke->fN;

		Stroke* queryT_stroke = new Stroke( f1, fN );
		fitted_segments->push_back( queryT_stroke );

		for( int f = f1; f <= fN; f++ )
		{
			math::position p = query_stroke->pts[ f-f1 ];
			p.set_x( p.x() * scale );
			p.set_y( p.y() * scale );
			p.set_z( p.z() * scale );

			queryT_stroke->pts.push_back( p );
		}
		iterative_closest_point( min_stroke, queryT_stroke, NUM_ICP );
	}
	double mean_dist = sum_dist / num_queries;
	return mean_dist;
}

static int fitClustersToString( 
	std::vector< int >* string, std::vector< StrokeCluster* >* cluster_list, 
	std::vector< StrokeCluster* >* retrieved_clusters, std::vector< StrokeCluster* >* fitted_clusters 
)
{
	if( string->empty() || retrieved_clusters->empty() )
	{
		return INT_MIN;
	}

	//
	char ascii_base = 'A';
	int i;

	CharString haystack;
	CharString needle;

	int text_len = (int)string->size();
	for( i=0; i < text_len; i++ )
	{
		appendValue( haystack, (char)( *string )[ i ] + ascii_base );
	}

	int pattern_len = (int)retrieved_clusters->size();
	for( i=0; i < pattern_len; i++ )
	{
		appendValue( needle, (char)( ( *retrieved_clusters )[ i ]->id ) + ascii_base );
	}

	//
	std::cout << "(*) Fitting clusters to string\n";
	std::cout << " - Text: " << haystack << "\n";
	std::cout << " - Pattern: " << needle << "\n";

	//
	Finder<CharString> finder( haystack );
	Pattern<CharString, Myers<>> pattern( needle );

	std::pair< int, int > max_substring;
	int max_score = INT_MIN;
	int min_score = -2;

	clear( finder );
    while( find(finder, pattern, min_score) )
	{
		while( findBegin(finder, pattern, getScore(pattern)) )
		{
			int begin = beginPosition( finder );
			int end = endPosition( finder );
			int score = getScore( pattern );

			if( score > max_score )
			{
				max_score = score;
				max_substring = std::make_pair( begin, end );
			}
		}
	}

	//
	fitted_clusters->clear();

	if( 0/*max_score != INT_MIN*/ )
	{
		std::cout << " - Found: ";

		for( i=max_substring.first; i < max_substring.second; i++ )
		{
			char c = getValue( haystack, i );
			int n = c - ascii_base;

			fitted_clusters->push_back( ( *cluster_list )[ n ] );

			std::cout << c << "(" << n << ")";
		}

		std::cout << "(score = " << max_score << ")\n";
	}
	else
	{
		for( i=0; i < pattern_len; i++ )
		{
			fitted_clusters->push_back( ( *retrieved_clusters )[ i ] );
		}
		std::cout << " - Not Found\n";
	}

	return max_score;
}

static inline bool sort_dists( std::pair<int, double>& lh, std::pair<int, double>& rh )
{
	return ( lh.second < rh.second );
}

static void inputStrokeAll()
{
	//
	UserTest* user_test = test_results[ test_results.size()-1 ];

	int num_pts = (int)stroke_points.size();

	double len = 0;
	for( int t=0; t < num_pts-1; t++ )
	{
		math::position p0 = stroke_points[ t ];
		math::position p1 = stroke_points[ t+1 ];

		math::vector v( p1-p0 );
		len += v.length();
	}

	user_test->num_sketch_frames = num_pts;
	user_test->sketch_length = len;
	std::copy( stroke_points.begin(), stroke_points.end(), std::inserter( user_test->sketch_points, user_test->sketch_points.begin() ) );

	//
	std::vector< Stroke* > rh_segments, rh_in_segments, rh_out_segments, rh_fit_segments;
	std::vector< Stroke* > lh_segments, lh_in_segments, lh_out_segments, lh_fit_segments;
	std::vector< Stroke* > rf_segments, rf_in_segments, rf_out_segments, rf_fit_segments;
	std::vector< Stroke* > lf_segments, lf_in_segments, lf_out_segments, lf_fit_segments;
	std::vector< Stroke* > hip_segments, hip_in_segments, hip_out_segments, hip_fit_segments;

	std::vector< StrokeCluster* > rh_out_clusters, lh_out_clusters, rf_out_clusters, lf_out_clusters, hip_out_clusters;
	std::vector< StrokeCluster* > rh_fit_clusters, lh_fit_clusters, rf_fit_clusters, lf_fit_clusters, hip_fit_clusters;

	findNearestClusters( &stroke_points, &rh_clusters, &rh_segments, &rh_in_segments, &rh_out_clusters );
	findNearestClusters( &stroke_points, &lh_clusters, &lh_segments, &lh_in_segments, &lh_out_clusters );
	findNearestClusters( &stroke_points, &rf_clusters, &rf_segments, &rf_in_segments, &rf_out_clusters );
	findNearestClusters( &stroke_points, &lf_clusters, &lf_segments, &lf_in_segments, &lf_out_clusters );
	findNearestClusters( &stroke_points, &hip_clusters, &hip_segments, &hip_in_segments, &hip_out_clusters );
	
	fitClustersToString( &rh_string, &rh_clusters, &rh_out_clusters, &rh_fit_clusters );
	fitClustersToString( &lh_string, &lh_clusters, &lh_out_clusters, &lh_fit_clusters );
	fitClustersToString( &rf_string, &rf_clusters, &rf_out_clusters, &rf_fit_clusters );
	fitClustersToString( &lf_string, &lf_clusters, &lf_out_clusters, &lf_fit_clusters );
	fitClustersToString( &hip_string, &hip_clusters, &hip_out_clusters, &hip_fit_clusters );

	double rh_dist = findNearestSegments( &rh_in_segments, &rh_fit_clusters, &rh_out_segments, &rh_fit_segments );
	double lh_dist = findNearestSegments( &lh_in_segments, &lh_fit_clusters, &lh_out_segments, &lh_fit_segments );
	double rf_dist = findNearestSegments( &rf_in_segments, &rf_fit_clusters, &rf_out_segments, &rf_fit_segments );
	double lf_dist = findNearestSegments( &lf_in_segments, &lf_fit_clusters, &lf_out_segments, &lf_fit_segments );
	double hip_dist = findNearestSegments( &hip_in_segments, &hip_fit_clusters, &hip_out_segments, &hip_fit_segments );
	
	/*
	double rh_dist = findNearestSegments( &rh_in_segments, &rh_out_clusters, &rh_out_segments, &rh_fit_segments );
	double lh_dist = findNearestSegments( &lh_in_segments, &lh_out_clusters, &lh_out_segments, &lh_fit_segments );
	double rf_dist = findNearestSegments( &rf_in_segments, &rf_out_clusters, &rf_out_segments, &rf_fit_segments );
	double lf_dist = findNearestSegments( &lf_in_segments, &lf_out_clusters, &lf_out_segments, &lf_fit_segments );
	double hip_dist = findNearestSegments( &hip_in_segments, &hip_out_clusters, &hip_out_segments, &hip_fit_segments );
	*/

	std::vector< std::pair<int, double> > dists;
	dists.push_back( std::make_pair(0, rh_dist) );
	dists.push_back( std::make_pair(1, lh_dist) );
	dists.push_back( std::make_pair(2, rf_dist) );
	dists.push_back( std::make_pair(3, lf_dist) );
	dists.push_back( std::make_pair(4, hip_dist) );

	std::sort( dists.begin(), dists.end(), sort_dists );

	for( int i=0; i < 3; i++ )
	{
		int type = dists[i].first;

		std::vector< Stroke* > *segments = 0, *in_segments = 0, *out_segments = 0, *fit_segments = 0;
		std::vector< StrokeCluster* > *out_clusters = 0, *fit_clusters = 0;

		switch( type ) {
		case 0:	
			segments = &rh_segments;
			in_segments = &rh_in_segments;
			out_clusters = &rh_out_clusters;
			fit_clusters = &rh_fit_clusters;
			out_segments = &rh_out_segments;	
			fit_segments = &rh_fit_segments;
			break;

		case 1:
			segments = &lh_segments;
			in_segments = &lh_in_segments;
			out_clusters = &lh_out_clusters;
			fit_clusters = &lh_fit_clusters;
			out_segments = &lh_out_segments;	
			fit_segments = &lh_fit_segments;
			break;

		case 2:
			segments = &rf_segments;
			in_segments = &rf_in_segments;
			out_clusters = &rf_out_clusters;
			fit_clusters = &rf_fit_clusters;
			out_segments = &rf_out_segments;	
			fit_segments = &rf_fit_segments;
			break;

		case 3:
			segments = &lf_segments;
			in_segments = &lf_in_segments;
			out_clusters = &lf_out_clusters;
			fit_clusters = &lf_fit_clusters;
			out_segments = &lf_out_segments;	
			fit_segments = &lf_fit_segments;
			break;

		case 4:
			segments = &hip_segments;
			in_segments = &hip_in_segments;
			out_clusters = &hip_out_clusters;
			fit_clusters = &hip_fit_clusters;
			out_segments = &hip_out_segments;	
			fit_segments = &hip_fit_segments;
			break;
		}

		QueryStroke* query = new QueryStroke;
		query_list.push_back( query );

		std::copy( segments->begin(), segments->end(), std::inserter( query->initial_segments, query->initial_segments.begin() ) );
		std::copy( in_segments->begin(), in_segments->end(), std::inserter( query->query_segments, query->query_segments.begin() ) );
		std::copy( fit_segments->begin(), fit_segments->end(), std::inserter( query->fitted_segments, query->fitted_segments.begin() ) );

		RetrievedMotion* retrieval = new RetrievedMotion;
		retrieval_list.push_back( retrieval );

//		std::copy( out_clusters->begin(), out_clusters->end(), std::inserter( retrieval->retrieved_clusters, retrieval->retrieved_clusters.begin() ) );
		std::copy( fit_clusters->begin(), fit_clusters->end(), std::inserter( retrieval->retrieved_clusters, retrieval->retrieved_clusters.begin() ) );
		std::copy( out_segments->begin(), out_segments->end(), std::inserter( retrieval->retrieved_segments, retrieval->retrieved_segments.begin() ) );
	}
}

static void inputStroke()
{
//	findNearestClusters( &stroke_points, &rh_clusters, &stroke_list, &query_stroke_list, &retrieved_cluster_list );
//	findNearestSegments( &query_stroke_list, &retrieved_cluster_list, &retrieved_stroke_list, &queryT_stroke_list );
}

 
//
class StrokeListener : public Listener {
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);

	//
	bool is_recording;
	std::queue< math::position > observed_points;
};

void StrokeListener::onInit(const Controller& controller) 
{
	std::cout << "(*) Leap: Initialized" << std::endl;
}

void StrokeListener::onConnect(const Controller& controller) 
{
	std::cout << "(*) Leap: Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_CIRCLE);
//	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
//	controller.enableGesture(Gesture::TYPE_SWIPE);

	is_recording = false;
}

void StrokeListener::onDisconnect(const Controller& controller) 
{
	//Note: not dispatched when running in a debugger.
	std::cout << "(*) Leap: Disconnected" << std::endl;
}

void StrokeListener::onExit(const Controller& controller) 
{
	std::cout << "(*) Leap: Exited" << std::endl;
}

void StrokeListener::onFrame(const Controller& controller) 
{
	const Frame frame = controller.frame();

	if( !frame.hands().empty() ) 
	{
		const Hand hand = frame.hands()[0];
		const FingerList fingers = hand.fingers();

		if( !fingers.empty() )
		{
			Vector v;

			EnterCriticalSection( &cs );

			if( !observed_points.empty() )
			{
				math::position pi = observed_points.back();
				double min_dist = DBL_MAX;

				int num_fingers = fingers.count();
				for( int j=0; j < num_fingers; j++ )
				{
					Vector vj = fingers[j].tipPosition();
					math::position pj( vj.x, vj.y, vj.z );

					double dist = math::vector( pj-pi ).length();
					if( dist < min_dist )
					{
						v = vj;
						min_dist = dist;
					}
				}
			}
			else
			{
				v = fingers[0].tipPosition();
			}

			math::position p( v.x, v.y, v.z );	
			observed_points.push( p );

			LeaveCriticalSection( &cs );

			// fingers.count
			// fingers[i].tipPosition
			// hand.sphereRadius
			// hand.palmPosition
			// hand.palmNormal
			// hand.direction.pitch/roll/yaw
		}
	}

	const GestureList gestures = frame.gestures();
	for( int g=0; g < gestures.count(); g++ )
	{
		Gesture gesture = gestures[ g ];

		switch( gesture.type() ) {
		case Gesture::TYPE_KEY_TAP :
			{
//				is_recording = !is_recording;
			}
			break;

		case Gesture::TYPE_CIRCLE:
			{
//				PostQuitMessage( 0 );
//				exit( 0 );
			}
			break;

		case Gesture::TYPE_SWIPE:
			{
			}
			break;

		case Gesture::TYPE_SCREEN_TAP :
			{
			}
			break;

		case Gesture::TYPE_INVALID :
			{
			}
			break;

		default :
			{
			}
			break;
		}
	}
}

void StrokeListener::onFocusGained(const Controller& controller) 
{
	std::cout << "(*) Leap: Focus Gained" << std::endl;
}

void StrokeListener::onFocusLost(const Controller& controller) 
{
	std::cout << "(*) Leap: Focus Lost" << std::endl;
}


//
static std::vector< Panel* > panel_list;
static Panel* selected_panel = 0;

static void destroyViewGrid()
{
	std::vector< Panel* >::iterator itor_p = panel_list.begin();
	while( itor_p != panel_list.end() )
	{
		Panel* panel = ( *itor_p ++ );
		delete panel;
	}
	panel_list.clear();

	selected_panel = 0;
}

static void createViewGrid( int left, int top, int right, int bottom )
{
	int width = right - left;
	int height = bottom - top;

	unsigned int panel_width = 250;		//width / num_cols;
	unsigned int panel_height = 250;	//panel_width / win_aspect_ratio;

	destroyViewGrid();

	//
	UserTest* user_test = test_results[ test_results.size()-1 ];
	
	int num_retrieved_segments = 0;
	int num_exact_segments = 0;
	int num_apprx_segments = 0;

	//
	int num_queries = (int)query_list.size();

	for( int r=0; r < num_queries; r++ )
	{
		QueryStroke* query = query_list[ r ];
		RetrievedMotion* retrieval = retrieval_list[ r ];

		std::vector< Stroke* >* initial_segments = &query->initial_segments;
		std::vector< Stroke* >* query_segments = &query->query_segments;
		std::vector< Stroke* >* fitted_segments = &query->fitted_segments;
		std::vector< Stroke* >* retrieved_segments = &retrieval->retrieved_segments;
		std::vector< StrokeCluster* >* retrieved_clusters = &retrieval->retrieved_clusters;

		int num_segments = (int)retrieved_segments->size();
		
		//
		num_retrieved_segments += num_segments;

		//
		for( int c=0; c < num_segments+1; c++ )
		{
			Panel* panel = new Panel;
			panel_list.push_back( panel );

			panel->setLeft( left + panel_width * c + 5);
			panel->setTop( win_height - top - panel_height * r - 10 );
			panel->setWidth( panel_width - 10 );
			panel->setHeight( panel_height - 20 );

			if( c == 0 )
			{
				panel->setSkeletalMotion( 0 );
				panel->setStartFrame( 0 );
				panel->setEndFrame( 0 );
				panel->setCurrentFrame( 0 );

				std::vector< Stroke* >::iterator itor_s = query_segments->begin();
				while( itor_s != query_segments->end() )
				{
					Stroke* segment = ( *itor_s ++ );

					panel->addPointSet( &segment->pts );
				}

				//
				float view_matrix[16], proj_matrix[16];

				glMatrixMode( GL_MODELVIEW );
				glPushMatrix();
				glLoadIdentity();
				glTranslatef( 0, -view_height/2, 0 );
				glGetFloatv( GL_MODELVIEW_MATRIX, view_matrix );
				glPopMatrix();

				//
				glMatrixMode( GL_PROJECTION );
				glPushMatrix();
				glLoadIdentity();
//				gluPerspective( fovy, 1, z_near, z_far );
				glOrtho( -view_width/2, view_width/2, -view_height/2, view_height/2, -view_distance, view_distance );
				glGetFloatv( GL_PROJECTION_MATRIX, proj_matrix );
				glPopMatrix();

				panel->setViewMatrix( view_matrix );
				panel->setProjMatrix( proj_matrix );
				panel->setAspectRatio( 1.0 );
				panel->setFovy( fovy );
				panel->setTheta( 0 );
				panel->setPhi( 0 );
			}
			else
			{
				Stroke* fit_segment = ( *fitted_segments )[ c-1 ];
				Stroke* out_segment = ( *retrieved_segments )[ c-1 ];
				StrokeCluster* out_cluster = ( *retrieved_clusters )[ c-1 ];

				panel->setSkeletalMotion( &skeletal_motion );
				panel->setStartFrame( out_segment->f1 );
				panel->setEndFrame( out_segment->fN );
				panel->setCurrentFrame( out_segment->f1 );
				panel->setDebugPoints( &out_segment->pts );

				camera_director.findBestConfiguration( panel );

				{
					int max_f1 = ( problem_f1 > out_segment->f1 ? problem_f1 : out_segment->f1 );
					int min_fN = ( problem_fN < out_segment->fN ? problem_fN : out_segment->fN );

					if( max_f1 <= min_fN )
					{
						panel->setBorderColor( 1, 0, 0 );
						panel->setBorderWidth( 5 );

						//
						num_exact_segments ++;

						user_test->exact_segments.push_back( std::make_pair( out_segment->f1, out_segment->fN ) );
					}
					else
					{
						bool is_found = false;

						std::vector< Stroke* >* out_segments = &out_cluster->members;
						std::vector< Stroke* >::iterator itor_s = out_segments->begin();
						while( itor_s != out_segments->end() )
						{
							Stroke* segment = ( *itor_s ++ );

							max_f1 = ( problem_f1 > segment->f1 ? problem_f1 : segment->f1 );
							min_fN = ( problem_fN < segment->fN ? problem_fN : segment->fN );

							if( max_f1 <= min_fN )
							{
								is_found = true;
								break;
							}
						}

						if( is_found )
						{
							panel->setBorderColor( 0, 0, 1 );
							panel->setBorderWidth( 5 );

							//
							num_apprx_segments ++;

							user_test->apprx_segments.push_back( std::make_pair( out_segment->f1, out_segment->fN ) );
						}
						else
						{
							panel->setBorderColor( 0, 0, 0 );
							panel->setBorderWidth( 3 );
						}
					}
				}
			}
		}
	}
	user_test->num_retrieved_segments = num_retrieved_segments;
	user_test->num_exact_segments = num_exact_segments;
	user_test->num_apprx_segments = num_apprx_segments;
}

static void updateViewGrid()
{
	std::vector< Panel* >::iterator itor_v = panel_list.begin();
	while( itor_v != panel_list.end() )
	{
		Panel* panel = ( *itor_v ++ );
		unsigned int f1 = panel->getStartFrame();
		unsigned int fN = panel->getEndFrame();
		unsigned int f = panel->getCurrentFrame();

		f ++;
		if( f > fN )	f = f1;

		panel->setCurrentFrame( f );
	}
}

static void drawViewGrid()
{
    glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
    glOrtho( 0, win_width, 0, win_height, -100, 100 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	std::vector< Panel* >::iterator itor_p = panel_list.begin();
	while( itor_p != panel_list.end() )
	{
		Panel* panel = ( *itor_p ++ );

		float border_width = panel->getBorderWidth();

		if( panel == selected_panel )
		{
			panel->setBorderWidth( 5 );	
		}
		panel->setPoseThickness( 5 );
		panel->renderAnimated();

		if( panel == selected_panel )
		{
			panel->setBorderWidth( border_width );
		}
	}

	//
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}


//
void startLeapMotion( int* argc, char** argv ) 
{
	atexit( finalize );

	initGLUT( argc, argv );
	initGL();
	initLeap();
	initStrokeDB();

	run();
}

void initLeap()
{
	InitializeCriticalSection( &cs );

	listener = new StrokeListener;
	controller = new Controller;
	controller->addListener( *listener );
}

void initGLUT( int* argc, char** argv )
{
	glutInit( argc, argv );
	glutInitWindowSize( win_width, win_height );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL );
    glutCreateWindow( "Motion Google" );

	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard ); 
	glutSpecialFunc( special );
	glutMouseFunc( mouse );
	glutMotionFunc( motion );
	glutTimerFunc( time_interval, timer, 1 );
	glutIdleFunc( idle );
}

void initGL()
{
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_NORMALIZE );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	GLfloat position[] = { 1.0f, 1.0f, 1.0f, 0.0f };		// directional source
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess = 100.0f;

	glLightfv( GL_LIGHT0, GL_POSITION, position );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
	glLightf( GL_LIGHT0, GL_SHININESS, shininess );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//
	float q[4] = { 0, 0, 0, 1 };
	track_ball.SetQuat( q );

	//
	glewInit();
}

void initStrokeDB()
{
	skeletal_motion.importFromBVH( PATH_BVH );
	
	//setupBoxingSkeleton( skeletal_motion.getSkeleton() );
	//setupBboySkeleton( original_motion.getSkeleton() );
	//setupCMU14Skeleton( original_motion.getSkeleton() );
	setupBasketballSkeleton( skeletal_motion.getSkeleton() );

	//
	loadClusters( "clusters_left_hand", &lh_clusters, &lh_strokes );
	loadClusters( "clusters_right_hand", &rh_clusters, &rh_strokes );
	loadClusters( "clusters_left_foot", &lf_clusters, &lf_strokes );
	loadClusters( "clusters_right_foot", &rf_clusters, &rf_strokes );
	loadClusters( "clusters_pelvis", &hip_clusters, &hip_strokes );
	
	//
	loadString( "string_left_hand", &lh_string );
	loadString( "string_right_hand", &rh_string );
	loadString( "string_left_foot", &lf_string );
	loadString( "string_right_foot", &rf_string );
	loadString( "string_pelvis", &hip_string );

	//
	setNewProblem(); 

	//
	int num_frames = skeletal_motion.getNumFrames();
	int num_lh_segments = (int)lh_strokes.size();
	int num_rh_segments = (int)rh_strokes.size();
	int num_lf_segments = (int)lf_strokes.size();
	int num_rf_segments = (int)rf_strokes.size();
	int num_hip_segments = (int)hip_strokes.size();

	std::cout << "# frames: " << num_frames << "\n";
	std::cout << "# lh segments: " << num_lh_segments << "\n";
	std::cout << "# rh segments: " << num_rh_segments << "\n";
	std::cout << "# lf segments: " << num_lf_segments << "\n";
	std::cout << "# rf segments: " << num_rf_segments << "\n";
	std::cout << "# hip segments: " << num_hip_segments << "\n";
}

void run()
{
	glutMainLoop();
}

void finalize()
{
	destroyViewGrid();

	removeStrokes();
	removeClusters();

	controller->removeListener( *listener );
	delete controller;
	delete listener;

	DeleteCriticalSection( &cs );

	//
	int i;

	std::ofstream os( PATH_TEST_RESULT, std::ios::out );
	if( os.is_open() )
	{
		std::vector< UserTest* >::iterator itor_t = test_results.begin();
		while( itor_t != test_results.end() )
		{
			UserTest* user_test = ( *itor_t ++ );

			os << user_test->f1 << std::endl;
			os << user_test->fN << std::endl;
			os << user_test->num_motion_frames << std::endl;
			os << user_test->motion_length << std::endl;

			os << user_test->num_sketch_frames << std::endl;	
			os << user_test->sketch_length << std::endl;

			for( i=0; i < user_test->num_sketch_frames; i++ )
			{
				os << user_test->sketch_points[ i ].x() << " ";
				os << user_test->sketch_points[ i ].y() << " ";
				os << user_test->sketch_points[ i ].z() << std::endl;
			}

			os << user_test->num_retrieved_segments << std::endl;

			os << user_test->num_exact_segments << std::endl;
			for( i=0; i < user_test->num_exact_segments; i++ )
			{
				os << user_test->exact_segments[ i ].first << " ";
				os << user_test->exact_segments[ i ].second << std::endl;
			}

			os << user_test->num_apprx_segments << std::endl;
			for( i=0; i < user_test->num_apprx_segments; i++ )
			{
				os << user_test->apprx_segments[ i ].first << " ";
				os << user_test->apprx_segments[ i ].second << std::endl;
			}

			os << std::endl;
		}
	}
}

void reshape( int w, int h )
{
	win_width = w;
	win_height = h;
	win_aspect_ratio = (float)w / (float)h;

	track_ball.SetWindowInfo( win_width, win_height, 0, 0 );
	drawing_tool.setWindowSize( win_width, win_height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
//	gluPerspective( fovy, win_aspect_ratio, z_near, z_far );
	glOrtho( -view_width/2 * win_aspect_ratio, view_width/2 * win_aspect_ratio, 
			 -view_height/2, view_height/2, -view_distance, view_distance );

	glViewport( 0, 0, win_width, win_height );

	if( search_mode == MOTION_RETRIEVAL )
	{
		destroyViewGrid();
		createViewGrid( 100, 0, win_width, win_height );
	}
}

static void setupLight()
{
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	GLfloat position[] = { 1.0f, 2.0f, 3.0f, 0.0f };		// directional source
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess = 100.0f;

	glLightfv( GL_LIGHT0, GL_POSITION, position );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
	glLightf( GL_LIGHT0, GL_SHININESS, shininess );
}

//

static void drawQueryMode()
{
	glViewport( 0, 0, win_width/2, win_height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glPushMatrix();
	gluPerspective( fovy, 1.0, z_near, z_far );
//	glOrtho( -view_width/2, view_width/2, -view_height/2, view_height/2, -view_distance, view_distance );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glPushMatrix();
	glTranslatef( 0, -view_height/2, -view_distance * 1.25 );

	//
	setupLight();

	drawing_tool.setColor( 0.5, 0.5, 0.5, 1 );
	drawing_tool.drawBox( math::position(0,0,0), math::vector(view_width,1,view_distance) );
	drawing_tool.drawBox( math::position(0,view_height,0), math::vector(view_width,1,view_distance) );
	drawing_tool.drawBox( math::position(-view_width/2,view_height/2,0), math::vector(1,view_height,view_distance) );
	drawing_tool.drawBox( math::position(+view_width/2,view_height/2,0), math::vector(1,view_height,view_distance) );
	drawing_tool.drawBox( math::position(0,view_height/2,-view_distance/2), math::vector(view_width,view_height,1) );

	double k = 1.0, L = 30, t = 0, R = 5.0;

	std::deque< math::position >::reverse_iterator itor_p = captured_points.rbegin();
	while( itor_p != captured_points.rend() )
	{
		math::position p = ( *itor_p ++ );

		double alpha = exp( k - k / (1 - (t/L)*(t/L) ) );
		double radius = exp( k - k / (1 - (t/L)*(t/L) ) ) * R;

		if( listener->is_recording )
			drawing_tool.setColor( 1, 0, 0, alpha );
		else
			drawing_tool.setColor( 0, 0, 1, alpha );

		drawing_tool.drawSphere( p, radius );

		if( alpha < 1e-4 )	break;

		t ++;

		//
		math::position pL(p), pB(p), pF(p);
		pL.set_x( -view_width/2 );
		pB.set_y( 0 );
		pF.set_z( -view_distance/2 );

		drawing_tool.setColor( 0.0, 0.05, 0.25, 1 );
		drawing_tool.drawSphere( pL, radius );
//		drawing_tool.drawSphere( pR, radius );
//		drawing_tool.drawSphere( pT, radius );
		drawing_tool.drawSphere( pB, radius );
		drawing_tool.drawSphere( pF, radius );
	}

	float stroke_radius = 2;

	int stroke_len = (int)stroke_points.size();
	if( stroke_len > 1 )
	{
		for( int i=0; i < stroke_len-1; i++ )
		{
			drawing_tool.setColor( 0.75, 0.25, 0.0, 1 );
			drawing_tool.drawSphere( stroke_points[i], stroke_radius );
			drawing_tool.drawCylinder( stroke_points[i], stroke_points[i+1], stroke_radius );

			//
			math::position pL1( stroke_points[i] ), pB1( stroke_points[i] ), pF1( stroke_points[i] );
			math::position pL2( stroke_points[i+1] ), pB2( stroke_points[i+1] ), pF2( stroke_points[i+1] );

			pL1.set_x( -view_width/2 );
			pL2.set_x( -view_width/2 );

			pB1.set_y( 0 );
			pB2.set_y( 0 );

			pF1.set_z( -view_distance/2 );
			pF2.set_z( -view_distance/2 );

			drawing_tool.setColor( 0.25, 0.05, 0.0, 1 );
			drawing_tool.drawCylinder( pL1, pL2, stroke_radius/2 );
			drawing_tool.drawCylinder( pB1, pB2, stroke_radius/2 );
			drawing_tool.drawCylinder( pF1, pF2, stroke_radius/2 );
		}
		drawing_tool.drawSphere( stroke_points[stroke_len-1], stroke_radius );
	}

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	glDisable( GL_LIGHTING );

	glLineWidth( 5 );
	drawing_tool.setColor( 0, 0, 0, 1 );
	drawing_tool.strokeRectangle( 0, 0, view_width, view_height, 0 );

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	//
	if( listener->is_recording )
	{
		int stroke_len = (int)stroke_points.size();

		char frame_str[128];
		sprintf( frame_str, "RECORDING A STROKE: %5d", stroke_len );
		drawing_tool.setColor( 1, 0, 0, 1 );
		drawing_tool.drawText( 0, 0, GLUT_BITMAP_TIMES_ROMAN_24, frame_str );
	}

	//
	glEnable( GL_LIGHTING );

	glViewport( 0, 0, win_width, win_height );

    glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
    glOrtho( 0, win_width, 0, win_height, -100, 100 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	motion_panel.setPoseThickness( 5 );
	motion_panel.renderAnimated();

    glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

static void drawResultMode()
{
	glViewport( 0, 0, win_width, win_height );

	drawViewGrid();
}

static void drawViewMode()
{
	//
	glEnable( GL_LIGHTING );

	glViewport( 0, 0, win_width, win_height );

    glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
    glOrtho( 0, win_width, 0, win_height, -100, 100 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	motion_panel.setPoseThickness( 5 );
	motion_panel.renderAnimated();

	result_panel.setPoseThickness( 5 );
	result_panel.renderAnimated();

    glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

void display()
{
	glClearDepth( 1 );
	glClearColor( 1, 1, 1, 1 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	char str[128];

	if( search_mode == STROKE_QUERY )
	{
		drawQueryMode();

		//
		drawing_tool.setColor( 0.75, 0.15, 0, 1 );
		
		sprintf( str, "Sketch: move your finger while pressing [SPACE] key" );
		drawing_tool.drawText( 0, win_height-60, GLUT_BITMAP_HELVETICA_18, str );

		sprintf( str, "Search: press [TAB] key" );
		drawing_tool.drawText( 0, win_height-80, GLUT_BITMAP_HELVETICA_18, str );
	}
	else if( search_mode == MOTION_RETRIEVAL )
	{
		drawResultMode();

		/*
		drawing_tool.setColor( 0.75, 0.15, 0, 1 );
		
		sprintf( str, "Enlarge: click mouse [LEFT] button on one of the panels" );
		drawing_tool.drawText( 0, win_height-60, GLUT_BITMAP_HELVETICA_18, str );

		sprintf( str, "Query: press [TAB] key" );
		drawing_tool.drawText( 0, win_height-80, GLUT_BITMAP_HELVETICA_18, str );
		*/
	}
	else if( search_mode == VIEW_RESULT )
	{
		drawViewMode();

		/*
		drawing_tool.setColor( 0.75, 0.15, 0, 1 );
		
		sprintf( str, "Back: press [TAB] key" );
		drawing_tool.drawText( 0, win_height-60, GLUT_BITMAP_HELVETICA_18, str );
		*/
	}

	//
	drawing_tool.setColor( 1, 0, 0, 1 );
	sprintf( str, "TAKE %02d", (int)test_results.size() );
	drawing_tool.drawText( 0, win_height-30, GLUT_BITMAP_TIMES_ROMAN_24, str );

	//
	glutSwapBuffers();
}

void timer( int timer_id )
{
	SHORT is_key_down = GetKeyState( VK_SPACE );
	if( !was_recording && ( is_key_down & ((SHORT)1 << 15) ) )
	{
		listener->is_recording = true;

		stroke_points.clear();
	}
	if( was_recording && !( is_key_down & ((SHORT)1 << 15) ) )
	{
		listener->is_recording = false;
	}

	//
	glutTimerFunc( time_interval, timer, timer_id );
}

void idle()
{
	static unsigned int count = 0;

	if( is_playing )
	{
		if( count % 3 == 0 )
		{
			//
			if( search_mode == STROKE_QUERY )
			{
				int f = motion_panel.getCurrentFrame();
				f ++;

				if( f > motion_panel.getEndFrame() )
				{
					f = motion_panel.getStartFrame();
				}

				motion_panel.setCurrentFrame( f );
			}

			if( search_mode == VIEW_RESULT )
			{
				if( motion_panel.getCurrentFrame() == motion_panel.getEndFrame() )
				{
					motion_panel.setCurrentFrame( motion_panel.getStartFrame() );
				}
				else
				{
					motion_panel.setCurrentFrame( motion_panel.getCurrentFrame()+1 );
				}

				if( result_panel.getCurrentFrame() == result_panel.getEndFrame() )
				{
					result_panel.setCurrentFrame( result_panel.getStartFrame() );
				}
				else
				{
					result_panel.setCurrentFrame( result_panel.getCurrentFrame()+1 );
				}
			}

			if( search_mode == MOTION_RETRIEVAL )
			{
				updateViewGrid();
			}

			current_frame ++;
		}
		count ++;
	}

	//
	if( search_mode == STROKE_QUERY )
	{
		was_recording = listener->is_recording;

		EnterCriticalSection( &cs );

		while( !listener->observed_points.empty() )
		{
			math::position p = listener->observed_points.front();
			listener->observed_points.pop();

			captured_points.push_back( p );

			if( listener->is_recording )
			{
				stroke_points.push_back( p );
			}
		}

		LeaveCriticalSection( &cs );

		while( captured_points.size() > NUM_CAPTURED_POINTS )
		{
			captured_points.pop_front();
		}
	}
	glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
	case 9:	// tab
		{
			if( search_mode == STROKE_QUERY )
			{
				search_mode = MOTION_RETRIEVAL;

				inputStrokeAll();
				createViewGrid( 0, 0, win_width, win_height );
			}
			else if( search_mode == MOTION_RETRIEVAL )
			{
				stroke_points.clear();
				captured_points.clear();

				removeStrokes();
				destroyViewGrid();

				setNewProblem();

				search_mode = STROKE_QUERY;
			}
			else if( search_mode == VIEW_RESULT )
			{
				search_mode = MOTION_RETRIEVAL;
			}
		}
		break;

	case 13:	// enter
		{
		}
		break;

	case 27:	// esc
		{
			PostQuitMessage( 0 );
			exit( 0 );
		}
		break;

	case 32:	// space
		{
//			is_playing = !is_playing;
		}
		break;
	}
	glutPostRedisplay();
}

void special( int key, int x, int y )
{
	switch(key){
		case GLUT_KEY_LEFT:
			{
				current_frame -= 1;
			}
			break;
		case GLUT_KEY_RIGHT:
			{
				current_frame += 1;
			}
			break;
		case GLUT_KEY_UP:
			{
				current_frame -= 10;
			}
			break;
		case GLUT_KEY_DOWN:
			{
				current_frame += 10;
			}
			break;
	}

	glutPostRedisplay();
}

void mouse( int button, int state, int x, int y )
{
	if( button == GLUT_LEFT_BUTTON ) 
	{
		if( state == GLUT_DOWN ) 
		{
			is_lbutton_down = true;
			track_x = x;
			track_y = y;

			if( search_mode == MOTION_RETRIEVAL )
			{
				Panel* picked_panel = 0;

				std::vector< Panel* >::iterator itor_p = panel_list.begin();
				while( itor_p != panel_list.end() )
				{
					Panel* panel = ( *itor_p ++ );

					int l = panel->getLeft();
					int t = panel->getTop();
					int w = panel->getWidth();
					int h = panel->getHeight();

					if( x > l && x < l+w && win_height-y < t && win_height-y > t-h )
					{
						picked_panel = panel;
						break;
					}
				}
				selected_panel = picked_panel;

				if( selected_panel )
				{
					search_mode = VIEW_RESULT;

					motion_panel.setLeft( 0 );
					motion_panel.setTop( win_height );
					
					result_panel = ( *selected_panel );

					result_panel.setLeft( win_width / 2 );
					result_panel.setTop( win_height );
					result_panel.setWidth( win_width / 2 );
					result_panel.setHeight( win_height );

					motion_panel.setCurrentFrame( motion_panel.getStartFrame() );
					result_panel.setCurrentFrame( result_panel.getStartFrame() );
				}
			}
			else if( search_mode == VIEW_RESULT )
			{
				search_mode = MOTION_RETRIEVAL;
			}
		}
		else 
		{
			is_lbutton_down = false;
		}
		return;
	}
	if( button == GLUT_RIGHT_BUTTON ) 
	{
		if( state == GLUT_DOWN ) 
		{
			is_rbutton_down = true;
			track_x = x;
			track_y = y;
		}
		else 
		{
			is_rbutton_down = false;
		}
		return;
	}
}

void motion( int x, int y )
{
	if( is_lbutton_down | is_rbutton_down ) 
	{
		if( is_lbutton_down ) 
		{	
			track_ball.Move( track_x, track_y, x, y );
		}
		else 
		{
			view_distance += (double)( y - track_y ) / win_height * 100.0f;
		}
		track_x = x;
		track_y = y;

		glutPostRedisplay();

		return;
	}
}

#pragma once

#include "MotionGraph.h"

class OrbitGraph
{
public:
	class Edge;

	// a cycle in the lower-level motion graph
	class Node
	{
	public:
		Node( std::vector< MotionGraph::Node* >* cycle )
		{
			this->cycle = *cycle;
		}
		~Node()
		{
		}

		inline unsigned int getPhase( MotionGraph::Node* curr_motion_node )
		{
			unsigned int i = 0;

			std::vector< MotionGraph::Node* >::iterator itor_n = cycle.begin();
			while( itor_n != cycle.end() )
			{
				MotionGraph::Node* motion_node = ( *itor_n ++ );
				if( motion_node == curr_motion_node )
				{
					break;
				}
				i ++;
			}
			return i;
		}

		inline MotionGraph::Node* stepInCycle( MotionGraph::Node* curr_motion_node )
		{
			bool is_found = false;

			std::vector< MotionGraph::Node* >::iterator itor_n = cycle.begin();
			while( itor_n != cycle.end() )
			{
				MotionGraph::Node* motion_node = ( *itor_n ++ );
				if( motion_node == curr_motion_node )
				{
					is_found = true;
					break;
				}
			}

			if( !is_found )
			{
				return 0;
			}
			else
			{
				if( itor_n == cycle.end() )
				{
					itor_n = cycle.begin();
				}
				return ( *itor_n );
			}
		}

		inline bool pathInCycle( MotionGraph::Node* curr_motion_node, MotionGraph::Node* dest_motion_node, std::vector< MotionGraph::Node* >* motion_path )
		{
			motion_path->clear();

			int cycle_size = (int)cycle.size(), num_steps = 1;

			MotionGraph::Node* motion_node = curr_motion_node;
			
			while( motion_node && motion_node != dest_motion_node && num_steps <= cycle_size )
			{
				motion_path->push_back( motion_node );
				motion_node = stepInCycle( motion_node );
				num_steps ++;
			}

			if( motion_node != dest_motion_node )
			{
				motion_path->clear();
				return false;
			}
			else
			{
				motion_path->push_back( motion_node );
				return true;
			}
		}

		inline void addNextEdge( Edge* edge )	{ next_edges.push_back( edge ); }
		inline void addPrevEdge( Edge* edge )	{ prev_edges.push_back( edge ); }

		inline std::vector< Edge* >*	getNextEdges()	{ return &next_edges; }
		inline std::vector< Edge* >*	getPrevEdges()	{ return &prev_edges; }

		inline bool removeNextEdge( Edge* edge )	
		{
			std::vector< Edge* >::iterator itor_next = next_edges.begin();
			while( itor_next != next_edges.end() )
			{
				Edge* e = ( *itor_next );
				if( e == edge )
				{
					next_edges.erase( itor_next );
					return true;
				}
				itor_next ++;
			}
			return false;
		}
		inline bool removePrevEdge( Edge* edge )
		{
			std::vector< Edge* >::iterator itor_prev = prev_edges.begin();
			while( itor_prev != prev_edges.end() )
			{
				Edge* e = ( *itor_prev );
				if( e == edge )
				{
					prev_edges.erase( itor_prev );
					return true;
				}
				itor_prev ++;
			}
			return false;
		}

		inline std::vector< MotionGraph::Node* >* getCycle()	{ return &cycle; }
		inline unsigned int getCycleSize()						{ return (unsigned int)cycle.size(); }

		// DIJKSTRA
		inline void dijkstraInit() {
			dijkstra_pred = NULL;
			dijkstra_dist = +FLT_MAX;
			dijkstra_color = 0;
		}
		inline void dijkstraSetPred( Node* pred )	{ dijkstra_pred = pred; }
		inline void dijkstraSetDist( float dist )	{ dijkstra_dist = dist; }

		inline Node* dijkstraGetPred()				{ return dijkstra_pred; }
		inline float dijkstraGetDist()				{ return dijkstra_dist; }

		inline void dijkstraWhite()					{ dijkstra_color = 0; }
		inline void dijkstraGray()					{ dijkstra_color = 1; }
		inline void dijkstraBlack()					{ dijkstra_color = 2; }
		inline bool dijkstraIsWhite()				{ return dijkstra_color==0; }
		inline bool dijkstraIsGray()				{ return dijkstra_color==1; }
		inline bool dijkstraIsBlack()				{ return dijkstra_color==2; }

	protected:
		std::vector< MotionGraph::Node* > cycle;

		std::vector< Edge* > next_edges;
		std::vector< Edge* > prev_edges;

		// DIJKSTRA (BFS)
		Node*			dijkstra_pred;
		float			dijkstra_dist;
		unsigned int	dijkstra_color;
	};

	// a path in the lower-level motion graph
	class Edge
	{
	public:
		Edge( Node* fn, Node* tn, std::vector< MotionGraph::Node* >* path )	
		{
			from_node = fn;
			to_node = tn;

			this->path = *path;
		}
		~Edge()
		{
		}

		inline std::vector< MotionGraph::Node* >* getPath()	{ return &path; }
		inline unsigned int getPathLength()					{ return (unsigned int)path.size(); }
		
		inline Node* getFromNode()	{ return from_node; }
		inline Node* getToNode()	{ return to_node; }

		inline MotionGraph::Node* startInPath()	{ return path[0]; }
		inline MotionGraph::Node* endInPath()	{ return path[ path.size()-1 ]; }

		inline MotionGraph::Node* stepInPath( MotionGraph::Node* curr_motion_node )
		{
			bool is_found = false;

			std::vector< MotionGraph::Node* >::iterator itor_n = path.begin();
			while( itor_n != path.end() )
			{
				MotionGraph::Node* motion_node = ( *itor_n ++ );
				if( motion_node == curr_motion_node )
				{
					is_found = true;
					break;
				}
			}

			if( !is_found )
			{
				return 0;
			}
			else
			{
				if( itor_n == path.end() )
				{
					return 0;
				}
				else
				{
					return ( *itor_n );
				}
			}
		}

		inline unsigned int getPhase( MotionGraph::Node* curr_motion_node )
		{
			unsigned int i = 0;

			std::vector< MotionGraph::Node* >::iterator itor_n = path.begin();
			while( itor_n != path.end() )
			{
				MotionGraph::Node* motion_node = ( *itor_n ++ );
				if( motion_node == curr_motion_node )
				{
					break;
				}
				i ++;
			}
			return i;
		}

	protected:
		std::vector< MotionGraph::Node* > path;

		Node* from_node;
		Node* to_node;
	};

	OrbitGraph( MotionGraph* motion_graph );
	virtual ~OrbitGraph();

	void initialize();
	void finalize();

	bool load( std::string path );
	bool save( std::string path );

	Node* addNode( std::vector< MotionGraph::Node* >* cycle );
	Node* getNode( unsigned int index );
	unsigned int getNodeIndex( Node* node );
	bool hasNode( Node* node );

	Edge* addEdge( Node* from_node, Node* to_node, std::vector< MotionGraph::Node* >* path );
	Edge* getEdge( unsigned int index );
	std::vector< Edge* >* findEdges( Node* from_node, Node* to_node );
	bool removeEdges( Node* from_node, Node* to_node );
	bool hasEdge( Edge* edge );

	bool findPath( Node* from_node, Node* to_node, std::deque< Node* >* orbit_path, std::deque< MotionGraph::Node* >* motion_path = 0 );

	inline std::vector< Node* >* getNodeList()	{ return &node_list; }
	inline std::vector< Edge* >* getEdgeList()	{ return &edge_list; }

	inline unsigned int getNumNodes()	{ return (unsigned int)node_list.size(); }
	inline unsigned int getNumEdges()	{ return (unsigned int)edge_list.size(); }

protected:
	void removeEdge( Edge* edge );

	MotionGraph* motion_graph;

	std::vector< Node* >				node_list;
	std::vector< Edge* >				edge_list;

	std::map< std::pair<Node*,Node*>, std::vector<Edge*>* >	edge_map;

	//
	void dijkstraInit();
	void dijkstraRelax( Node* node );
	Node* dijkstraExtractMin();
	std::vector< Node* >::iterator dijkstraFindInQ( Node* node );
	static bool dijkstraCompare( Node* lh, Node* rh );

	Node*					dijkstra_source;
	Node*					dijkstra_target;
	std::set< Node* >		dijkstra_pool;
	std::vector< Node* >	dijkstra_Q;
};

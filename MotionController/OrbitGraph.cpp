#include "OrbitGraph.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>

OrbitGraph::OrbitGraph( MotionGraph* motion_graph )
{
	this->motion_graph = motion_graph;
}

OrbitGraph::~OrbitGraph()
{
	finalize();
}

void OrbitGraph::initialize()
{
	finalize();
}

void OrbitGraph::finalize()
{
	std::vector< Node* >::iterator itor_node = node_list.begin();
	while( itor_node != node_list.end() )
	{
		Node* node = ( *itor_node ++ );
		delete node;
	}
	std::vector< Edge* >::iterator itor_edge = edge_list.begin();
	while( itor_edge != edge_list.end() )
	{
		Edge* edge = ( *itor_edge ++ );
		delete edge;
	}
}

bool OrbitGraph::load( std::string path )
{
	finalize();

	std::ifstream is( path.c_str(), std::ios::in );
	if( !is.is_open() )
	{
		return false;
	}

	// node list
	unsigned int num_nodes = 0, n;
	is >> num_nodes;

	for( n=0; n < num_nodes; n++ )
	{
		unsigned int cycle_size = 0, c;
		is >> cycle_size;

		std::vector< MotionGraph::Node* > cycle;
		for( c=0; c < cycle_size; c++ )
		{
			unsigned int index = 0;
			is >> index;

			MotionGraph::Node* cycle_node = motion_graph->getNode( index );
			if( cycle_node == 0 )
			{
				finalize();
				return false;
			}
			cycle.push_back( cycle_node );
		}

		addNode( &cycle );
	}

	// edge list
	unsigned int num_edges = 0, e;
	is >> num_edges;

	for( e=0; e < num_edges; e++ )
	{
		unsigned int from_index = 0, to_index = 0;
		is >> from_index >> to_index;

		Node* from_node = getNode( from_index );
		Node* to_node = getNode( to_index );
		if( !from_node || !to_node )
		{
			finalize();
			return false;
		}

		unsigned int path_len = 0, p;
		is >> path_len;

		std::vector< MotionGraph::Node* > path;
		for( p=0; p < path_len; p++ )
		{
			unsigned int index = 0;
			is >> index;

			MotionGraph::Node* path_node = motion_graph->getNode( index );
			if( path_node == 0 )
			{
				finalize();
				return false;
			}
			path.push_back( path_node );
		}

		addEdge( from_node, to_node, &path );
	}

	return true;
}

bool OrbitGraph::save( std::string path )
{
	std::ofstream os( path.c_str(), std::ios::out );
	if( !os.is_open() )
	{
		return false;
	}

	// node list
	unsigned int num_nodes = (unsigned int)node_list.size();
	os << num_nodes << std::endl;

	std::vector< Node* >::iterator itor_node = node_list.begin();
	while( itor_node != node_list.end() )
	{
		Node* node = ( *itor_node ++ );

		unsigned int cycle_size = node->getCycleSize();
		os << cycle_size << std::endl;

		std::vector< MotionGraph::Node* >* cycle = node->getCycle();
		std::vector< MotionGraph::Node* >::iterator itor_n = cycle->begin();
		while( itor_n != cycle->end() )
		{
			MotionGraph::Node* cycle_node = ( *itor_n ++ );
			unsigned int index = motion_graph->getNodeIndex( cycle_node );

			os << index << " ";
		}
		os << std::endl;
	}
	os << std::endl;

	// edge list
	unsigned int num_edges = (unsigned int)edge_list.size();
	os << num_edges << std::endl;

	std::vector< Edge* >::iterator itor_edge = edge_list.begin();
	while( itor_edge != edge_list.end() )
	{
		Edge* edge = ( *itor_edge ++ );

		Node* from_node = edge->getFromNode();
		Node* to_node = edge->getToNode();
		unsigned int from_index = getNodeIndex( from_node );
		unsigned int to_index = getNodeIndex( to_node );

		os << from_index << " " << to_index << std::endl;

		unsigned int path_len = edge->getPathLength();
		os << path_len << std::endl;

		std::vector< MotionGraph::Node* >* path = edge->getPath();
		std::vector< MotionGraph::Node* >::iterator itor_n = path->begin();
		while( itor_n != path->end() )
		{
			MotionGraph::Node* path_node = ( *itor_n ++ );
			unsigned int index = motion_graph->getNodeIndex( path_node );

			os << index << " ";
		}
		os << std::endl;
	}
	os << std::endl;

	os.close();

	return true;
}

OrbitGraph::Node* OrbitGraph::addNode( std::vector< MotionGraph::Node* >* cycle )
{
	Node* node = new Node( cycle );
	node_list.push_back( node );

	return node;
}

OrbitGraph::Node* OrbitGraph::getNode( unsigned int index )
{
	if( index < (unsigned int)node_list.size() )
	{
		return node_list[ index ];
	}
	return NULL;
}

unsigned int OrbitGraph::getNodeIndex( Node* node )
{
	unsigned int index = 0;

	std::vector< Node* >::iterator itor_node = node_list.begin();
	while( itor_node != node_list.end() )
	{
		Node* n = ( *itor_node ++ );
		if( n == node )
		{
			break;
		}
		index ++;
	}
	return index;
}

bool OrbitGraph::hasNode( Node* node )
{
	std::vector< Node* >::iterator itor_node = node_list.begin();
	while( itor_node != node_list.end() )
	{
		Node* n = ( *itor_node ++ );
		if( n == node )
		{
			return true;
		}
	}
	return false;
}

OrbitGraph::Edge* OrbitGraph::addEdge( Node* from_node, Node* to_node, std::vector< MotionGraph::Node* >* path )
{
	Edge* edge = new Edge( from_node, to_node, path );
	edge_list.push_back( edge );

	//
	from_node->addNextEdge( edge );
	to_node->addPrevEdge( edge );

	//
	std::vector< Edge* >* edges = findEdges( from_node, to_node );
	if( !edges )
	{
		edges = new std::vector< Edge* >;
		edge_map[ std::make_pair(from_node, to_node) ] = edges;
	}
	edges->push_back( edge );

	//
	return edge;
}

OrbitGraph::Edge* OrbitGraph::getEdge( unsigned int index )
{
	if( index < (unsigned int)edge_list.size() )
	{
		return edge_list[ index ];
	}
	return NULL;
}

std::vector< OrbitGraph::Edge* >* OrbitGraph::findEdges( Node* from_node, Node* to_node )
{
	std::map< std::pair<Node*,Node*>, std::vector<Edge*>* >::iterator itor_edge = edge_map.find( std::make_pair(from_node,to_node) );
	if( itor_edge != edge_map.end() )
	{
		return itor_edge->second;
	}
	return NULL;
}

bool OrbitGraph::removeEdges( Node* from_node, Node* to_node )
{
	std::map< std::pair<Node*,Node*>, std::vector<Edge*>* >::iterator itor_edge = edge_map.find( std::make_pair(from_node,to_node) );
	if( itor_edge != edge_map.end() )
	{
		std::vector< Edge* >* edges = itor_edge->second;
		std::vector< Edge* >::iterator itor_e = edges->begin();
		while( itor_e != edges->end() )
		{
			Edge* edge = ( *itor_e ++ );
			removeEdge( edge );
		}
		edge_map.erase( itor_edge );
		delete edges;

		return true;
	}
	else
	{
		return false;
	}
}

void OrbitGraph::removeEdge( OrbitGraph::Edge* edge )
{
	bool is_found = false;
	std::vector< Edge* >::iterator itor_e = edge_list.begin();
	while( itor_e != edge_list.end() )
	{
		Edge* e = ( *itor_e );
		if( edge == e )
		{
			edge_list.erase( itor_e );
			is_found = true;
			break;
		}
		itor_e ++;
	}

	if( is_found )
	{
		Node* from_node = edge->getFromNode();
		Node* to_node = edge->getToNode();

		from_node->removeNextEdge( edge );
		to_node->removePrevEdge( edge );

		delete edge;
	}
}

bool OrbitGraph::hasEdge( OrbitGraph::Edge* edge )
{
	std::vector< Edge* >::iterator itor_e = edge_list.begin();
	while( itor_e != edge_list.end() )
	{
		Edge* e = ( *itor_e );
		if( edge == e )
		{
			return true;
		}
	}
	return false;
}

//

bool OrbitGraph::findPath( Node* from_node, Node* to_node, std::deque< Node* >* orbit_path, std::deque< MotionGraph::Node* >* motion_path /*=0*/ )
{
	// initialization
	dijkstraInit();

//	from_node->dijkstraSetDist( 0.0f );
//	from_node->dijkstraSetPred( NULL );
//	from_node->dijkstraGray();

	dijkstra_Q.clear();
	dijkstra_Q.push_back( from_node );
	std::make_heap( dijkstra_Q.begin(), dijkstra_Q.end(), dijkstraCompare );

	// find the shortest path to target node
	bool is_from_node_visited = false;
	while( !dijkstra_Q.empty() )
	{
		Node* node = dijkstraExtractMin();
		if( node == from_node )
		{
			if( !is_from_node_visited )
			{
				is_from_node_visited = true;
			}
			else
			{
				node->dijkstraBlack();
			}
		}
		else
		{
			node->dijkstraBlack();
		}
		dijkstraRelax( node );
	}

	// record the path
	if( to_node )
	{
		orbit_path->clear();

		Node* v = to_node;
		do
		{
			orbit_path->push_front( v );
			v = v->dijkstraGetPred();
		}
		while( v != from_node && v != NULL );

		if( v == from_node )
		{
			orbit_path->push_front( from_node );
		}
		else
		{
			orbit_path->clear();
			return false;
		}
	}
	else
	{
		return false;
	}

	// convert higher-level orbit path into lower-level motion path
	if( motion_path && orbit_path->size() > 1 )
	{
		motion_path->clear();

		Node* prev_orbit_node = 0;

		std::deque< Node* >::iterator itor_n = orbit_path->begin();
		while( itor_n != orbit_path->end() )
		{
			Node* orbit_node = ( *itor_n ++ );
			Node* next_orbit_node = ( itor_n != orbit_path->end() ? *itor_n : 0 );

			if( prev_orbit_node )	// is not the first node		
			{
				std::vector< Edge* >* prev_orbit_edges = findEdges( prev_orbit_node, orbit_node );
				Edge* prev_orbit_edge = ( *prev_orbit_edges )[ 0 ];

				std::vector< MotionGraph::Node* >* prev_motion_nodes = prev_orbit_edge->getPath();
				std::vector< MotionGraph::Node* >::iterator itor_pmn = prev_motion_nodes->begin();
				while( itor_pmn != prev_motion_nodes->end() )
				{
					MotionGraph::Node* prev_motion_node = ( *itor_pmn ++ );
					motion_path->push_back( prev_motion_node );
				}

				if( next_orbit_node )	// is not the last node (or, is a middle node)
				{
					std::vector< Edge* >* next_orbit_edges = findEdges( orbit_node, next_orbit_node );
					Edge* next_orbit_edge = ( *next_orbit_edges )[ 0 ];

					std::vector< MotionGraph::Node* >* next_motion_nodes = next_orbit_edge->getPath();

					MotionGraph::Node* prev_last_node = ( *prev_motion_nodes )[ prev_motion_nodes->size()-1 ];
					MotionGraph::Node* next_first_node = ( *next_motion_nodes )[ 0 ];

					std::vector< MotionGraph::Node* > path_in_cycle;
					bool is_found = orbit_node->pathInCycle( prev_last_node, next_first_node, &path_in_cycle );
					if( is_found )
					{
						bool is_first = true;

						std::vector< MotionGraph::Node* >::iterator itor_cmn = path_in_cycle.begin();
						while( itor_cmn != path_in_cycle.end() )
						{
							MotionGraph::Node* cycle_motion_node = ( *itor_cmn ++ );
							if( is_first )
							{
								is_first = false;
							}
							else
							{
								if( itor_cmn != path_in_cycle.end() )
								{
									motion_path->push_back( cycle_motion_node );
								}
							}
						}
					}
					else
					{
						motion_path->clear();
						return false;
					}
				}
			}

			prev_orbit_node = orbit_node;
		}
	}
	return true;
}

//

bool OrbitGraph::dijkstraCompare( Node* lh, Node* rh )
{
	if( lh->dijkstraGetDist() > rh->dijkstraGetDist() )
	{
		return true;
	}
	return false;
}

std::vector< OrbitGraph::Node* >::iterator OrbitGraph::dijkstraFindInQ( Node* node )
{
	std::vector< Node* >::iterator i = dijkstra_Q.begin();
	while( i != dijkstra_Q.end() )
	{
		Node* n = ( *i );
		if( n == node )
		{
			break;
		}
		i ++;
	}
	return i;
}

void OrbitGraph::dijkstraInit()
{
	dijkstra_Q.clear();
	dijkstra_source = NULL;

	std::vector< Node* >::iterator itor_node = node_list.begin();
	while( itor_node != node_list.end() )
	{
		Node* node = ( *itor_node ++ );
		node->dijkstraInit();
	}
}

void OrbitGraph::dijkstraRelax( Node* node )
{
	float node_dist = node->dijkstraGetDist();

	std::vector< Edge* >* edges = node->getNextEdges();
	std::vector< Edge* >::iterator itor_e = edges->begin();
	while( itor_e != edges->end() )
	{
		Edge* edge = ( *itor_e ++ );
		Node* adj_node = edge->getToNode();

		if( adj_node->dijkstraIsBlack() )
		{
			continue;
		}
		
		float dist = (float)edge->getPath()->size();	//(float)math::vector( adj_pos-pos ).length();
		float new_dist = node_dist + dist;

		if( adj_node->dijkstraIsWhite() )
		{
			adj_node->dijkstraSetDist( new_dist );
			adj_node->dijkstraSetPred( node );
			adj_node->dijkstraGray();

			dijkstra_Q.push_back( adj_node );
			std::push_heap( dijkstra_Q.begin(), dijkstra_Q.end(), dijkstraCompare );
		}
		else
		{
			float old_dist = adj_node->dijkstraGetDist();
			if( old_dist > new_dist )
			{
				adj_node->dijkstraSetDist( new_dist );
				adj_node->dijkstraSetPred( node );

				std::vector< Node* >::iterator index_Q = dijkstraFindInQ( adj_node );
				if( index_Q != dijkstra_Q.end() )
				{
					std::push_heap( dijkstra_Q.begin(), index_Q+1, dijkstraCompare );
				}
				else
				{
					assert( false );
				}
			}
		}
	}
}

OrbitGraph::Node* OrbitGraph::dijkstraExtractMin()
{
	if( dijkstra_Q.empty() )
	{
		return NULL;
	}
	Node* n = dijkstra_Q.front();
	std::pop_heap( dijkstra_Q.begin(), dijkstra_Q.end(), dijkstraCompare );
	dijkstra_Q.pop_back();

	return n;
}

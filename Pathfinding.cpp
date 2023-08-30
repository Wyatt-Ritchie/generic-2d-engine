#include <vector>
#include <unordered_map>
#include <queue>
struct GraphNode
{
	// Each node points to an adjacent node

	std::vector<GraphNode*> mAdjacent;
};

struct Graph
{
	// Graph containing nodes

	std::vector<GraphNode*> mNodes;
};

struct WeightedEdge
{
	// Nodes that ar connected by this edge

	struct WeightedGraphNode* mFrom;

	struct WeightedGraphNode* mTo;

	// Weight of the edge

	float mWeight;
};

struct WeightedGraphNode
{
	// Vector storing outgoing edges much like the regular
	// graph node, except these have a weight

	std::vector<WeightedEdge*> mEdges;
};

struct WeightedGraph
{
	// weighted graph containing weighted nodes

	std::vector<WeightedGraphNode*> mWeightedNodes;
};

// This unordered map contains sets of parent nodes and child nodes
using NodeToParentMap =
std::unordered_map<const GraphNode*, const GraphNode*>;

// Breadth-First Search implementation

bool BFS(const Graph& graph, const GraphNode* start,
	const GraphNode* goal, NodeToParentMap& outMap)
{
	// bool for identifying a path found
	bool pathFound = false;

	// Nodes to be considered
	std::queue<const GraphNode*> q;

	// Emplace first  node
	q.emplace(start);

	// Loop for performing search
	while (!q.empty())
	{
		// Deque a node

		const GraphNode* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}

		// Enque adjacent nodes that aren't already in the queue
		for (const GraphNode* node : current->mAdjacent)
		{
			// if the parent is null, it hasn't been enqueued
			// except for the start node
			const GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start)
			{
				// Enqueue this node, setting its parent
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}

	return pathFound;
};

// Function for computing the heuristic
float ComputeHeuristic(const WeightedGraphNode* a, const WeightedGraphNode* b)
{
	// fill this in later
	// likely use manhattan or euclidean distances.
	// Will need to get positional information from nodes
	return 0.0f;
}

// GBFS scractch data struct
/*
*  mParentEdge is a weighted edge poiter that points to the parent
*  mHeuristic is a floating point number that represents the heuristic
*  mInOpenSet identifies the node as being in the open set or not
*  mInClosedSet identifies the node as being in the closed set or not
*/

struct GBFSScratch
{
	const WeightedEdge* mParentEdge = nullptr;

	float mHeuristic = 0.0f;

	bool mInOpenSet = false;

	bool mInClosedSet = false;
};

// Like in BFS we define a map containing pointers to nodes, but
// this time the value is an instance of GBFSScratch
using GBFSMap =
std::unordered_map<const WeightedGraphNode*, GBFSScratch>;

// Greedy Breadth-First Search implementation
bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, GBFSMap& outMap)
{
	// start by creating a vector for the open set
	std::vector<const WeightedGraphNode*> openSet;

	// set current node to the start node and add to the closed set
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	// main loop of GBFS

	do
	{
		// adding adjacent nodes to the open set
		for (const WeightedEdge* edge : current->mEdges)
		{
			// get scratch data for this node
			GBFSScratch& data = outMap[edge->mTo];

			// Add only if it's not in the closed set
			if (!data.mInClosedSet)
			{
				// Set the adjacent node's parent edge
				data.mParentEdge = edge;
				if (!data.mInOpenSet)
				{
					// compute heuristic for this node and
					// place in open set
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}
		if (openSet.empty())
		{
			break; // there is no path from start to goal
		}

		// Find the lowest cost node in the open set
		auto iter = std::min_element(openSet.begin(), openSet.end(), 
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b)
			{
				return outMap[a].mHeuristic < outMap[b].mHeuristic;
			});

		// Set to current and moved from open to closed
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;

	} while (current != goal);

	// return booling for path found
	return (current == goal) ? true : false;
};

// Scratch data for the A* pathfinding algorithm
struct AStarScratch
{
	const WeightedEdge* mParentEdge = nullptr;

	float mHeuristic = 0.0f;

	float mActualFromStart = 0.0f;

	bool mInOpenSet = false;

	bool mInClosedSet = false;
};

using AStarMap =
std::unordered_map<const WeightedGraphNode*, AStarScratch>;

// A* Search implementation
bool AStarSearch(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, AStarMap& outMap)
{
	// start by creating a vector for the open set
	std::vector<const WeightedGraphNode*> openSet;

	// set current node to the start node and add to the closed set
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	// main loop of GBFS

	do
	{
		// adding adjacent nodes to the open set
		for (const WeightedEdge* edge : current->mEdges)
		{
			const WeightedGraphNode* neighbor = edge->mTo;

			// get scratch data for this node
			AStarScratch& data = outMap[neighbor];

			// Add only if it's not in the closed set
			if (!data.mInClosedSet)
			{
				if (!data.mInOpenSet)
				{
					// Not in the open set, so parent is current
					data.mParentEdge = edge;
					// compute heuristic for this node and
					// place in open set
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					// Actual cost is the parent's plus cost of traversing edge

					data.mActualFromStart = outMap[current].mActualFromStart +
						edge->mWeight;

					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
			}
		}
		if (openSet.empty())
		{
			break; // there is no path from start to goal
		}

		// Find the lowest cost node in the open set
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b)
			{
				float fOfA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
				float fOfB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
				return fOfA < fOfB;
			});

		// Set to current and moved from open to closed
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;

	} while (current != goal);

	// return booling for path found
	return (current == goal) ? true : false;
};


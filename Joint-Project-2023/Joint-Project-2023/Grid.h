#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<list>
#include <queue>
#include <array>

class Node
{
public:
	Node(bool _walkable, int _gridX, int _gridY) : 
		walkable(_walkable),
		gridX(_gridX),
		gridY(_gridY)
	{

	};


	void addNeighbour(int t_cellId) { m_neighbours.push_back(t_cellId); }
	std::vector<int>& neighbours() { return m_neighbours; }
	bool isMarked() const { return marked; }
	void setMarked(bool t_val) { marked = t_val; }
	void setPreviousCellID(int t_previousCellID) { m_previousCellId = t_previousCellID; }
	void setWalkable(bool t_value) { walkable = t_value; }
	bool isWalkable() { return walkable; }
	int getPreviousCellID() { return m_previousCellId; }
	void setID(int t_id) { m_id = t_id; }
	int getID() { return m_id; }

	int gridX; //position x
	int gridY; //position y

private:
	bool walkable{ false }; //can tile be walked through
	bool marked{ false };
	int m_id = -1;
	int m_previousCellId = -1;
	std::vector<int> m_neighbours;
	//Node parent;
};

class PathFinder
{
public:
	PathFinder(std::vector<Node>& t_nodeGrid, int t_mapWidth, int t_mapHeight, int t_gridNodeSize) :
		nodeGrid(t_nodeGrid),
		mapWidth(t_mapWidth),
		mapHeight(t_mapHeight),
		gridNodeSize(t_gridNodeSize)
	{

	}

	/// <summary>
	/// Finds a path using breathdfirst search.
	/// Searches for neighbours and loops to search every neighbours neighbour until we find the destination.
	/// Add the neighbours to a search queue, where they will take turns searching for their neighbours.
	/// When a neighbour is found that hasnt been checked by another node (ie, marked == false), add it to the search queue
	/// for its turn when it comes around.
	/// </summary>
	/// <param name="_nodeNum"></param>
	/// <returns></returns>
	std::vector<int> findPath(Node t_startNode, Node t_endNode)
	{
		std::queue<Node> searchQueue; //neighbours that will search their neighbours
		std::vector<int> pathQueue;
		bool goalReached = false;
		std::vector<int> neighbours;
		int startCellId = -1;
		int destinationCellId = -1;

		startCellId = t_startNode.getID();
		destinationCellId = t_endNode.getID();

		nodeGrid[startCellId].setMarked(true);
		searchQueue.push(nodeGrid[startCellId]);

		while (searchQueue.size() > 0 && !goalReached)
		{
			int searchID = nodeGrid[searchQueue.front().getID()].getID();
			neighbours = nodeGrid[searchID].neighbours();
			for (int i = 0; i < neighbours.size() && !goalReached; i++)
			{
				if (neighbours[i] == destinationCellId)
				{
					goalReached = true;
					nodeGrid[destinationCellId].setPreviousCellID(searchQueue.front().getID());
				}
				else if (nodeGrid[neighbours[i]].isMarked() == false && nodeGrid[neighbours[i]].isWalkable() == true)
				{
					nodeGrid[neighbours[i]].setPreviousCellID(searchQueue.front().getID());
					nodeGrid[neighbours[i]].setMarked(true);
					searchQueue.push(nodeGrid[neighbours[i]]);
				}
			}
			searchQueue.pop();
		}

		int pathID = destinationCellId;
		int previousCellID = nodeGrid[pathID].getPreviousCellID();

		while (previousCellID != -1)
		{
			sf::RectangleShape pathBlock;
			pathBlock.setFillColor(sf::Color::Red);
			pathBlock.setSize({ static_cast<float>(gridNodeSize),static_cast<float>(gridNodeSize) });
			pathBlock.setPosition(nodeGrid[previousCellID].gridX, nodeGrid[previousCellID].gridY);
			m_pathShapes.push_back(pathBlock);

			pathQueue.push_back(previousCellID);
			pathID = previousCellID;
			previousCellID = nodeGrid[pathID].getPreviousCellID();
		}

		for (int i = 0; i < nodeGrid.size(); i++)
		{
			nodeGrid[i].setMarked(false);
			nodeGrid[i].setPreviousCellID(-1);
		}

		return pathQueue;
	}

	std::vector<sf::RectangleShape>& returnPathShapes() { return m_pathShapes; }

private:
	std::vector<Node>& nodeGrid;
	std::vector<sf::RectangleShape> m_pathShapes;
	int mapWidth, mapHeight; //map width x height

	int gridNodeSize; //each tile is 32x 32 , should use 2 varibles but keeps simple
};


class Grid
{
public:
	Grid()
	{

	};
	void createGrid(int _worldX, int _worldY) //creates grid, use on initialise 
	{
		mapWidth = _worldX; //sets map width in pixel
		mapHeight = _worldY; //sets map height in pixels

		int rows = _worldY / gridNodeSize;  //identifies the amount of rows
		int cols = _worldX / gridNodeSize;  //identifies the amount of columns
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				nodeGrid.push_back(Node(true, j * gridNodeSize, i * gridNodeSize)); //pushes a new node with passable and x and y and gives id
			}
		}
		//Setup id's first
		int max = rows * cols;
		for (int i = 0; i < max; i++)
		{
			nodeGrid[i].setID(i);
			
		}
		//setup neighbours of Id's
		for (int i = 0; i < max; i++)
		{
			searchNeighbours(i);
		}
		
	};

	/// <summary>
	/// sets the impassable bool when there is a collision block on our map
	/// </summary>
	/// <param name="_nodeX">x pos of collisions block</param>
	/// <param name="_nodeY">y pos of collisions block</param>
	void setImpassable(int _nodeX, int _nodeY)
	{
		nodeGrid.at(nodeFromWorldPoint(sf::Vector2f(_nodeX, _nodeY))).setWalkable(false); //reads node position and sets it to impassable for collisions
	};

	int nodeFromWorldPoint(sf::Vector2f _pos)
	{
		int percentX = (_pos.x) / gridNodeSize; //gets grid on x scale
		int percentY = (_pos.y) / gridNodeSize; //gets grid on y scale

		int x = percentY * (mapWidth / gridNodeSize) + percentX; //converts the row and column to the grid number

		return x; //returns grid
	}

	//returns collision
	sf::FloatRect getCollisionSquare(int _nodeNum)
	{
		if (!nodeGrid.at(_nodeNum).isWalkable())
		{
			return sf::FloatRect(nodeGrid.at(_nodeNum).gridX
				, nodeGrid.at(_nodeNum).gridY
				, gridNodeSize
				, gridNodeSize);
		}
	}

	/// <summary>
	/// finds neighbours of a given node and adds them to their neighbour vector
	/// Also assigns the neighbours node a previous cell id that is the original node we send in here so we can backtrack
	/// </summary>
	/// <param name="_node">node id</param>
	void searchNeighbours(int t_nodeId)

	{
		const int MAX_ROWS = (mapHeight / gridNodeSize);
		const int MAX_COLS = (mapWidth / gridNodeSize);

		int row = t_nodeId / MAX_COLS;
		int col = t_nodeId % MAX_COLS;
		int neighbourIndex = -1;

		//This neighbour algoritihim prioritises vertical/hjorizontal then diagonal
		std::array<int, 9> directionOrder
		{
			1,3,5,7,0,2,6,8 //exclude 4 since that is our cell
		};
		for (auto& direction: directionOrder) {

			int n_row = row + ((direction % 3) - 1); // Neighbor row
			int n_col = col + ((direction / 3) - 1); // Neighbor column

			// Check the bounds:
			if (n_row >= 0 && n_row < MAX_ROWS && n_col >= 0 && n_col < MAX_COLS) {
				neighbourIndex = n_row * MAX_COLS + n_col;
				nodeGrid[t_nodeId].neighbours().push_back(neighbourIndex);
			}
		}
	};

	std::queue<int> findPath(Node t_startNode, Node t_endNode)
	{
		PathFinder pathFinder(nodeGrid, mapWidth, mapHeight, gridNodeSize);
		std::vector<int> path = pathFinder.findPath(t_startNode,t_endNode);
		std::queue<int>pathQueue;
		for (int i = path.size() - 1; i >= 0; i--) //enemy follows the path as a queue so reverse vector since in a queue its first in fist out
		{
			pathQueue.push(path[i]);
		}
		//m_pathShapes = pathFinder.returnPathShapes();
		return pathQueue;
	}

	Node& getNode(int t_nodeId) 
	{ 
		return nodeGrid[t_nodeId]; 
	}

	//std::vector<sf::RectangleShape>& returnPathShapes() { return m_pathShapes; }

private:

	std::vector<Node> nodeGrid; //stores all nodes in a grid
	//std::vector<sf::RectangleShape> m_pathShapes;

	int mapWidth, mapHeight; //map width x height

	int gridNodeSize = 32; //each tile is 32x 32 , should use 2 varibles but keeps simple
};






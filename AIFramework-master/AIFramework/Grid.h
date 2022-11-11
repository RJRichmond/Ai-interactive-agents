#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_map>

class GridNode 
{
public:
	int GridPosX = 0;
	int GridPosY = 0;
	int TravelCost = 0;

	GridNode* Neighbours[4];
	sf::RectangleShape GridSquare;
	GridNode* NodeCameFrom;
	sf::Vector2f Midpoint;

	GridNode()
	{
		for (int i = 0; i < 4; i++)
		{
			Neighbours[i] = nullptr;
		}
		height = 60;
		width = 60;
		GridPosX = 0;
		GridPosY = 0;
		TravelCost = 1;
		Midpoint = sf::Vector2f(0, 0);
		GridSize = sf::Vector2f(60, 60);
		NodeCameFrom = nullptr;
	}

	void SetGridTile(int i, int j) 
	{
		// This is what I use for debugging if there is problems with the grid or the pathfinding.
		GridSquare.setSize(GridSize);
		GridSquare.setOutlineColor(sf::Color::White); 
		GridSquare.setOutlineThickness(1);
		GridSquare.setFillColor(sf::Color::Transparent);
		GridSquare.setPosition(i * height, j * width);
		

		int MidpointX = i * height;
		int MidpointY = j * height;

		Midpoint = sf::Vector2f((MidpointX + 30), (MidpointY + 30));
		GridPosY = i;
		GridPosX = j;
		
	}

	void ChangeGridColor() 
	{
		
		GridSquare.setOutlineColor(sf::Color::Red);
	}

	void SetAmmoTileColor() 
	{
		GridSquare.setOutlineColor(sf::Color::Yellow);
	}

	void SetWall() 
	{
		TravelCost = 10000;
		GridSquare.setFillColor(sf::Color::White);
	}
private:
	float width;
	float height;
	sf::Vector2f GridSize;
};

class Grid 
{
public:

	GridNode* AmmoTiles[2];

	GridNode* AgentLocation[2];

	GridNode* HideLocations[4];

	std::vector<GridNode*> CurrentPath;

	void CreateGrid() 
	{
		//Create grid sets the grid up ready for use such as making all of the grid nodes, getting the neighbours. Hardcoding any positons such as ammo and hiding spots.
		for (int i = 0; i < 10; i++) 
		{
			for (int j = 0; j < 10; j++) 
			{
				Grid[i][j] = new GridNode();
				Grid[i][j]->SetGridTile(i, j);
			}
		}

		for (int i = 0; i < 10; i++) 
		{
			for (int j = 0; j < 10; j++) 
			{
				getNeighbours(*Grid[i][j]);
			}
		}

		for (int j = 0; j < 2; j++)
		{
			AmmoTiles[j] = nullptr;
			AgentLocation[j] = nullptr;
		}

		for (int i = 0; i < 4; i++) 
		{
			HideLocations[i] = nullptr;
		}

		//Hardcoding ammo tiles
		AmmoTiles[0] = Grid[0][9];
		AmmoTiles[0]->SetAmmoTileColor();
		AmmoTiles[1] = Grid[9][0];
		AmmoTiles[1]->SetAmmoTileColor();

		//Correcting movement on second agent start
		AgentLocation[1] = Grid[9][5];

		//Hardcoding cover spots.
		HideLocations[0] = Grid[2][2];
		HideLocations[1] = Grid[2][7];

		HideLocations[2] = Grid[7][2];
		HideLocations[3] = Grid[7][7];

		//setting sprites.
		filename = "assets\\ArenaMap.png";
		colourTint = sf::Color::White;

		texture.loadFromFile(filename);
		texture.setSmooth(true);
		
		sprite.setTexture(texture);
		
		sprite.setColor(colourTint);

		CreateLayout();
	}

	void DestroyGrid() 
	{
		for (int i = 0; i < 10; i++) 
		{
			for (int j = 0; j < 10; j++) 
			{
				delete Grid[i][j];
			}
		}
	}

	void CreateLayout() 
	{
		//This function creates the layout of the map by setting the walls (high travel cost nodes).
		//Create left side of map
		Grid[3][1]->SetWall();
		Grid[3][2]->SetWall();
		Grid[3][3]->SetWall();
		Grid[2][3]->SetWall();
		Grid[1][3]->SetWall();

		Grid[3][6]->SetWall();
		Grid[3][7]->SetWall();
		Grid[3][8]->SetWall();
		Grid[2][6]->SetWall();
		Grid[1][6]->SetWall();
		

		//Create Right side of map

		Grid[6][1]->SetWall();
		Grid[6][2]->SetWall();
		Grid[6][3]->SetWall();
		Grid[7][3]->SetWall();
		Grid[8][3]->SetWall();

		Grid[6][6]->SetWall();
		Grid[6][7]->SetWall();
		Grid[6][8]->SetWall();
		Grid[7][6]->SetWall();
		Grid[8][6]->SetWall();


	}

	GridNode* GetNode(int x, int y) 
	{
		return Grid[x][y];
	}

	void getNeighbours(GridNode& Node) 
	{
		//Get Neighbours of each node by using the x and y positions of each.
		if (Node.GridPosY != 0)
		{
			Node.Neighbours[0] = Grid[Node.GridPosY - 1][Node.GridPosX];
		}

		if (Node.GridPosX != 9)
		{
			Node.Neighbours[1] = Grid[Node.GridPosY][Node.GridPosX + 1];
		}

		if (Node.GridPosY != 9)
		{
			Node.Neighbours[2] = Grid[Node.GridPosY + 1][Node.GridPosX];
		}

		if (Node.GridPosX != 0)
		{
			Node.Neighbours[3] = Grid[Node.GridPosY][Node.GridPosX - 1];
		}
	}

	void Draw(sf::RenderWindow& window)
	{

		window.draw(sprite);
		/*
		This is used for testing to draw the grid, not used in the finished build
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				window.draw(Grid[i][j]->GridSquare);
			}

		}
		*/
		
	}

	void AStarPathFinding(GridNode* StartPoint, GridNode* GoalPoint) 
	{
		//My A* pathfinding works by using a priority queue to sort the pairs by lowest to highest as well as a vector of searched nodes and unordered map to keep track of the costs of each node.
		std::priority_queue<std::pair<int, GridNode*>, std::vector<std::pair<int, GridNode*>>, std::greater<std::pair<int, GridNode*>>> newQueue;
		std::vector<GridNode*> SearchedNodes;
		std::unordered_map<GridNode*, int> Cost;
		std::vector<GridNode*> PathFound;
		CurrentPath.clear();

		//Pushes the start node into the queue pairing with the priority of 0, it then sets the current node to it sets up the travel and cost in the map.
		newQueue.push(std::make_pair(0, StartPoint));
		GridNode* CurrentNode = newQueue.top().second;

		CurrentNode->TravelCost = 0;
		SearchedNodes.push_back(CurrentNode);
		Cost[CurrentNode] = 0;

		//While the queue isnt empty get the current node, remove it from the queue, check to see if its the goal.
		while (!newQueue.empty()) 
		{
			CurrentNode = newQueue.top().second;
			SearchedNodes.push_back(CurrentNode);
			newQueue.pop();

			if (CurrentNode == GoalPoint) 
			{
				//If its the goal we then follow the nodes came from until we reach the beginning pushing the path into a vector which gets put into a another vector used for the soldier.
				int NewTravelCost = 0;
				while (CurrentNode != StartPoint) 
				{
					CurrentNode->ChangeGridColor();
					PathFound.push_back(CurrentNode);
					CurrentNode = CurrentNode->NodeCameFrom;
					NewTravelCost += Cost[CurrentNode];
				}
				PathFound.push_back(CurrentNode);

				int PathSize = PathFound.size();
				

				for (int i = 0; i < PathSize; i++) 
				{
					
					CurrentPath.push_back(PathFound.back());
					PathFound.pop_back();
				}
				break;
			}
			//For each neighbour the node has get the new cost by getting the cost of the currentnode + the neighbours travel cost.
			for (int k = 0; k < 4; k++) 
			{
				if (CurrentNode->Neighbours[k] != nullptr) 
				{
					int newCost = Cost[CurrentNode] + CurrentNode->Neighbours[k]->TravelCost;
					//If the neighbours cost is found in the map or the newcost is lower then the cost of the neighbour. It sets the cost of the neighbour in the map and check to see if its in the search nodes vector.
					if (Cost.find(CurrentNode->Neighbours[k]) == Cost.end() || newCost < Cost[CurrentNode->Neighbours[k]]) 
					{
						Cost[CurrentNode->Neighbours[k]] = newCost;
						if (std::find(SearchedNodes.begin(), SearchedNodes.end(), CurrentNode->Neighbours[k]) != SearchedNodes.end()) 
						{}
						else 
						{
							//If its not it gets the priority which is the newcost plus the heuristic and sets the camefrom node and emplaces the pair of priority and the neighbour in the queue.
							int Priority = newCost + (std::abs(CurrentNode->Neighbours[k]->GridPosX - GoalPoint->GridPosX) + std::abs(CurrentNode->Neighbours[k]->GridPosY - GoalPoint->GridPosY));
							CurrentNode->Neighbours[k]->NodeCameFrom = CurrentNode;
							newQueue.emplace(std::make_pair(Priority, CurrentNode->Neighbours[k]));
						}
						
					}
				}
			}
			
		}

		PathFound.clear();

	}
private:
	GridNode* Grid[10][10];

	std::string filename;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Color colourTint;
};
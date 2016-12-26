/*
AstarMap.cpp
Created by Chengtao Wang on 12/22/2016
*/
#include "AstarMap.h"

AstarMap::AstarMap(int row, int col) : GridMap(row, col)
{
	cout << "Now in astarmap!\n";
	cameFrom = new int[this->row * this->col];
}

void AstarMap::generatePath()
{
	//clean the former result
	cleanResult();
	//check the start and goal exist
	if (this->startStatus == 0 || this->goalStatus == 0) {
		cout << "You miss the start point or goal point, please check again!\n";
		return;
	}
	cout << "Use A star algorithm to generate the path!\n";
	bool pathExist = 0;
	vector<int> closedSet, openSet;
	int nodeNumber = this->row * this->col;
	openSet.push_back(this->startNode);
	float *gScore, *fScore;
	gScore = new float[nodeNumber];
	fScore = new float[nodeNumber];
	for (int i = 0; i < nodeNumber; i++) {
		gScore[i] = 9999;
		fScore[i] = 9999;
	}
	gScore[this->startNode] = 0;
	fScore[this->startNode] = heuristicCost(startNode, goalNode);

	while (!openSet.empty()) {
		//current = the node in openSet having the least fscore value.
		int current;
		float leastf = 9999;
		for (int i = 0; i < openSet.size(); i++) {
			if (fScore[openSet[i]] < leastf) {
				leastf = fScore[openSet[i]];
				current = openSet[i];
			}
		}

		if (current == this->goalNode) {
			pathExist = 1;
			drawPath(current);
			break;
			//draw path here
		}

		//Remove current from openSet
		vector<int>::iterator it = find(openSet.begin(), openSet.end(), current);
		openSet.erase(it);
		//Add current to closedSet
		closedSet.push_back(current);
		vector<int> neighbors = getNeighbor(current);

		for (int i = 0; i < neighbors.size(); i++) {
			vector<int>::iterator it2 = find(closedSet.begin(), closedSet.end(), neighbors[i]);
			if (it2 == closedSet.end()) {
				float tentativeGScore = gScore[current] + heuristicCost(current, neighbors[i]);
				vector<int>::iterator it3 = find(openSet.begin(), openSet.end(), neighbors[i]);
				if (it3 == openSet.end()) {
					openSet.push_back(neighbors[i]); //Discover new node
				}
				else if (tentativeGScore >= gScore[neighbors[i]]) { continue; }//Not a better path
				this->cameFrom[neighbors[i]] = current;
				gScore[neighbors[i]] = tentativeGScore;
				fScore[neighbors[i]] = tentativeGScore + heuristicCost(neighbors[i], current);
			}
		}
	}
	
	if (pathExist == 0) {
		cout << "There is no path in this map!\n";
	}

	delete[] gScore;
	delete[] fScore;
	gScore = nullptr;
	fScore = nullptr;
}

float AstarMap::heuristicCost(int from, int to)
{
	//Compute the heuristic cost between two nodes
	int *coor_f, *coor_t;
	float cost;
	coor_f = this->getCoor(from);
	coor_t = this->getCoor(to);
	cost = sqrt(pow(static_cast<float>(coor_f[0] - coor_t[0]), 2) + pow(static_cast<float>(coor_f[1] - coor_t[1]), 2));
	return cost;
}

int AstarMap::getNode(int i, int j)
{
	int node; //node begin from 0
	node = i * this->col + j;
	return node;
}

int* AstarMap::getCoor(int node)
{
	int* coor;
	coor = new int[2];
	coor[0] = node / this->col;
	coor[1] = node % this->col;
	return coor;
}

vector<int> AstarMap::getNeighbor(int current)
{
	//uncomment all to enable 8 neighbors
	vector<int> neighbors;
	int *coor;
	coor = getCoor(current);
	/*
	if (coor[0] > 0 && coor[1] > 0 && this->grids[coor[0] - 1][coor[1] - 1] != Obstacle) {
		neighbors.push_back(getNode(coor[0] - 1, coor[1] - 1)); 
	}
	*/
	if (coor[0] > 0 && this->grids[coor[0] - 1][coor[1]] != Obstacle) {
		neighbors.push_back(getNode(coor[0] - 1, coor[1]));
	}
	/*
	if (coor[0] > 0 && coor[1] + 1 < this->col && this->grids[coor[0] - 1][coor[1] + 1] != Obstacle) {
		neighbors.push_back(getNode(coor[0] - 1, coor[1] + 1));
	}
	*/
	if (coor[1] > 0 && this->grids[coor[0]][coor[1] - 1] != Obstacle) {
		neighbors.push_back(getNode(coor[0], coor[1] - 1));
	}
	if (coor[1] + 1 < this->col && this->grids[coor[0]][coor[1] + 1] != Obstacle) {
		neighbors.push_back(getNode(coor[0], coor[1] + 1));
	}
	/*
	if (coor[0] + 1 < this->row && coor[1] > 0 && this->grids[coor[0] + 1][coor[1] - 1] != Obstacle) {
		neighbors.push_back(getNode(coor[0] + 1, coor[1] - 1));
	}
	*/
	if (coor[0] + 1 < this->row && this->grids[coor[0] + 1][coor[1]] != Obstacle) {
		neighbors.push_back(getNode(coor[0] + 1, coor[1]));
	}
	/*
	if (coor[0] + 1 < this->row && coor[1] + 1 < this->col && this->grids[coor[0] + 1][coor[1] + 1] != Obstacle) {
		neighbors.push_back(getNode(coor[0] + 1, coor[1] + 1));
	}
	*/
	return neighbors;
}

void AstarMap::drawPath(int current)
{
	vector<int> path;
	path.push_back(current);
	while ( current != this->startNode)
	{
		cout << "Path generated: " << current << "\n";
		current = this->cameFrom[current];
		path.push_back(current);
	}
	cout << "Get the Path!\n";

	//draw path on the map
	for (int i = 0; i < path.size(); i++) {
		int *coor;
		coor = getCoor(path[i]);
		if (this->grids[coor[0]][coor[1]] == Free) {
			this->image(this->square[coor[0]][coor[1]]) = Vec3b(255, 0, 0);
		}
	}
}

void AstarMap::cleanResult()
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (this->grids[i][j] == Free) {
				this->image(this->square[i][j]) = Vec3b(255, 255, 255);
				rectangle(this->image, this->square[i][j], Scalar(255, 0, 0), 1);
			}
		}
	}

	for (int i = 0; i < this->row * this->col; i++) {
		this->cameFrom[i] = 0;
	}
}

AstarMap::~AstarMap()
{
	delete[] cameFrom;
	cameFrom = nullptr;
}
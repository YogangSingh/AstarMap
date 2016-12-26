/*
AstarMap.h
Created by Chengtao Wang on 12/22/2016
*/
#ifndef AstarMap_h
#define AstarMap_h

#include "GridMap.h"
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>

class AstarMap : public GridMap
{
public:
	AstarMap(int row, int col);
	virtual ~AstarMap();

protected:
	int getNode(int i, int j);
	int* getCoor(int node);
	float heuristicCost(int from, int to);
	vector<int> getNeighbor(int current);
	void generatePath();
	int* cameFrom;
	void drawPath(int current);
	void cleanResult();
};

#endif /* AstarMap_h */

/*
GridMap.h
Created by Chengtao Wang on 12/20/2016
*/
#ifndef GridMap_h
#define GridMap_h

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

enum Status { Free, Obstacle, Start, Goal };

class GridMap
{
public:
	GridMap(int row, int col);
	void ShowGrid();
	virtual ~GridMap();

protected:
	Status** grids;
	Rect** square;
	int row, col;
	//record whether start and goal has been selected
	bool startStatus, goalStatus;
	int startNode, goalNode;
	string winName;
	Mat3b image;
	Rect button, button_clean;
	void cleanGrid();
	virtual void generatePath(); //We do nothing here, our subclass will override.
	void callBackFunc(int event, int x, int y);
	static void callBackFunc(int event, int x, int y, int flags, void* userdata);

};

#endif /* GridMap_h */

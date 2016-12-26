/*
GridMap.cpp
Created by Chengtao Wang on 12/20/2016
*/
#include "GridMap.h"

GridMap::GridMap(int row, int col)
{
	this->row = row;
	this->col = col;
	this->startStatus = 0;
	this->goalStatus = 0;
	//generate image
	this->image = Mat3b(30 * row + 60, 30 * col, Vec3b(255, 255, 255));
	this->button = Rect(0, 0, this->image.cols, 30);
	this->button_clean = Rect(0, 30 + 30 * row, this->image.cols, 30);
	this->image(this->button) = Vec3b(200, 200, 200);
	this->image(this->button_clean) = Vec3b(200, 200, 200);
	const string buttonText = "Start!";
	const string cleanText = "Clean!";
	putText(this->image(this->button), buttonText, Point(this->button.width*0.45, this->button.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	putText(this->image(this->button_clean), cleanText, Point(this->button_clean.width*0.45, this->button_clean.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	//generate grids,square
	this->grids = new Status*[row];
	this->square = new Rect*[row];
	for (int i = 0; i < row; i++) {
		this->grids[i] = new Status[col];
		this->square[i] = new Rect[col];
		for (int j = 0; j < col; j++) {
			this->grids[i][j] = Free;
			this->square[i][j] = Rect(30 * j, 30 + 30 * i, 30, 30);
			rectangle(this->image, this->square[i][j], Scalar(255, 0, 0), 1);
			//cout << i << " ," << j << " is: " << grids[i][j] << "\n";
		}
	}
}

void GridMap::ShowGrid()
{
	this->winName = "The Map";
	namedWindow(this->winName);
	setMouseCallback(this->winName, callBackFunc, this);
	imshow(this->winName, this->image);
	waitKey();
}

void GridMap::cleanGrid()
{
	Rect cleanSquare(0, 30, this->image.cols, 30 * this->row);
	this->image(cleanSquare) = Vec3b(255, 255, 255);
	this->startStatus = 0;
	this->goalStatus = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			this->grids[i][j] = Free;
			rectangle(this->image, this->square[i][j], Scalar(255, 0, 0), 1);

		}
	}

}

void GridMap::generatePath() {
	cout << "Path can not be generated in GridMap program!\n";
	//just do nothing
}

void GridMap::callBackFunc(int event, int x, int y)
{
	if (event == EVENT_LBUTTONDOWN) //set obstacle, set free
	{
		if (this->button.contains(Point(x, y)))
		{
			cout << "Started!" << endl;
			generatePath();
			rectangle(this->image, this->button, Scalar(0, 0, 255), 2);
		}
		if (this->button_clean.contains(Point(x, y)))
		{
			cleanGrid();
			cout << "Cleaned!" << endl;
			rectangle(this->image, this->button_clean, Scalar(0, 0, 255), 2);
		}
		for (int i = 0; i < this->row; i++) {
			for (int j = 0; j < this->col; j++) {
				if (this->square[i][j].contains(Point(x, y)))
				{
					if (this->grids[i][j] == Free) {
						this->image(this->square[i][j]) = Vec3b(100, 100, 100);
						this->grids[i][j] = Obstacle;
						cout << "The grid " << i + 1 << "," << j + 1 << " is Obstacle!\n";
					}
					else if (this->grids[i][j] == Obstacle) {
						this->image(this->square[i][j]) = Vec3b(255, 255, 255);
						rectangle(this->image, this->square[i][j], Scalar(255, 0, 0), 1);
						this->grids[i][j] = Free;
						cout << "Free the grid " << i + 1 << "," << j + 1 << " !\n";
					}
					else if (this->grids[i][j] == Start) {
						this->image(this->square[i][j]) = Vec3b(100, 100, 100);
						this->grids[i][j] = Obstacle;
						this->startStatus = 0;
						cout << "The start " << i + 1 << "," << j + 1 << " changed to Obstacle!\n";
					}
					else if (this->grids[i][j] == Goal) {
						this->image(this->square[i][j]) = Vec3b(100, 100, 100);
						this->grids[i][j] = Obstacle;
						this->goalStatus = 0;
						cout << "The goal " << i + 1 << "," << j + 1 << " changed to Obstacle!\n";
					}
				}
			}
		}
	}
	if (event == EVENT_LBUTTONDBLCLK) //set start and goal
	{
		if (this->startStatus == 0 || this->goalStatus == 0) {
			for (int i = 0; i < this->row; i++) {
				for (int j = 0; j < this->col; j++) {
					if (this->square[i][j].contains(Point(x, y))) {
						if (this->startStatus == 0) {
							this->image(this->square[i][j]) = Vec3b(0, 255, 255);
							this->grids[i][j] = Start;
							cout << "The grid " << i + 1 << "," << j + 1 << " is the Start point!\n";
							this->startNode = i * this->col + j;
							//cout << "startNode:" << this->startNode << "\n";
							this->startStatus = 1;
						}
						else if (this->goalStatus == 0) {
							this->image(this->square[i][j]) = Vec3b(255, 255, 0);
							this->grids[i][j] = Goal;
							cout << "The grid " << i + 1 << "," << j + 1 << " is the Goal point!\n";
							this->goalNode = i * this->col + j;
							//cout << "goalNode:" << this->goalNode << "\n";
							this->goalStatus = 1;
						}
					}
				}
			}
		}
	}
	if (event == EVENT_LBUTTONUP) //make the button nicer
	{
		rectangle(this->image, this->button, Scalar(200, 200, 200), 2);
		rectangle(this->image, this->button_clean, Scalar(200, 200, 200), 2);
	}

	imshow(this->winName, this->image);
	waitKey(1);
}

void GridMap::callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	static_cast<GridMap*>(userdata)->callBackFunc(event, x, y);
}

GridMap::~GridMap()
{
	for (int i = 0; i < this->row; i++) {
		delete[] grids[i];
		delete[] square[i];
	}
	delete[] grids;
	delete[] square;
	grids = nullptr;
	square = nullptr;
}
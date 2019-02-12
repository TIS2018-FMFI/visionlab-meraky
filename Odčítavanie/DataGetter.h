#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cstdio>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


using namespace cv;
using namespace std;


#pragma once
class DataGetter
{
	Mat input;
	Mat gray;
	Mat thresh;
	Mat ROI;
	int number;
	String vysledokString;
	double vysledokDouble;
	int optimalWidth = 0;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	int Digits[10][7] = { {1, 1, 1, 0, 1, 1, 1 },	//0
						  { 0, 0, 1, 0, 0, 1, 0 },	//1
						  { 1, 0, 1, 1, 1, 0, 1 },	//2
						  { 1, 0, 1, 1, 0, 1, 1 },	//3
						  { 0, 1, 1, 1, 0, 1, 0 },	//4
						  { 1, 1, 0, 1, 0, 1, 1 },	//5
						  { 1, 1, 0, 1, 1, 1, 1 },	//6
						  { 1, 0, 1, 0, 0, 1, 0 },	//7
						  { 1, 1, 1, 1, 1, 1, 1 },	//8
						  { 1, 1, 1, 1, 0, 1, 1 } };	//9





public:
	DataGetter();
	DataGetter(Mat input);

	double getData();
	int analyzeImage(Mat roi, vector<Rect> boundRect2, int i);

	struct contour_sorter {

		bool operator()(const vector<Point>& a, const vector<Point>& b) {
			Rect ra(boundingRect(a));
			Rect rb(boundingRect(b));

			return (ra.x < rb.x);
		}
	};

	~DataGetter();
};


#include "stdafx.h"
#include "DataGetter.h"


DataGetter::DataGetter()
{
}

DataGetter::DataGetter(Mat input)
{
	this->input = input;
}

double DataGetter::getData()
{
	vysledokString = "";

	cvtColor(input, gray, COLOR_BGR2GRAY);		//turns the image to grayscale
	blur(gray, gray, Size(3, 3));				//smooths the gray image

	//imshow("?", input);
	//waitKey(0);

	threshold(gray, thresh, 58, 100, THRESH_BINARY);		//thresholds the image, cropping away the things we don't need

//	imshow("?", thresh);
//	waitKey(0);


	findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));		//finds all the contours on the image

	std::sort(contours.begin(), contours.end(), contour_sorter());			//sort the contours from left to right because it's not sure, they will be found that way

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (size_t i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);
	}

	//using the above 10 lines, we find all the bounding boxes of every contours

	RNG rng(12345);

	int size = contours.size();

	for (size_t i = 0; i < size; i++) {
		if ((boundRect[i].height < 10) || (boundRect[i].width < 10)) {
			for (size_t j = i; j < size - 1; j++) {
				contours[j] = contours[j + 1];
				boundRect[j] = boundRect[j + 1];
			}
			contours.pop_back();
			boundRect.pop_back();
			i--;
			size--;
		}
	}

	//using the above for cycle, we throw away every contours which are too small (the function findContours sometimes returns unused contours)

	vector<vector<Point> > contours_poly2(contours.size());
	vector<Rect> boundRect2(contours.size());
	vector<Point2f>center2(contours.size());
	vector<float>radius2(contours.size());

	for (size_t i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly2[i], 3, true);
		boundRect2[i] = boundingRect(Mat(contours_poly2[i]));
		minEnclosingCircle(contours_poly2[i], center2[i], radius2[i]);
	}

	//we find all bounding boxes again (although we cropped away the ones we don't need, it's much safer to find them again)

	for (size_t i = 0; i < contours.size(); i++) {
		if (optimalWidth == 0 && boundRect2[i].width > 20) {
			optimalWidth = boundRect2[i].width;
			continue;
		}

		if (boundRect2[i].width < 20) {
			continue;
		}
		else {
			boundRect2[i].width = optimalWidth;
		}
	}

	//we set up an optimal width because sometimes the bounding boxes of the last digits are way wider, so the algorithm used to process all the segments won't work


	if (boundRect2.size() > 0) {

		for (int i = 0; i < boundRect2.size() - 1; i++) {
			if (((boundRect2[i].x < boundRect2[i + 1].x) && (boundRect2[i].y < boundRect2[i + 1].y)) &&
				((boundRect2[i].x + boundRect2[i].width > boundRect2[i + 1].x + boundRect2[i + 1].width) && (boundRect2[i].y + boundRect2[i].height > boundRect2[i + 1].y + boundRect2[i + 1].height)))
			{
				for (int j = i + 1; j < boundRect2.size() - 1; j++) {
					boundRect2[j] = boundRect2[j + 1];
				}
				boundRect2.pop_back();
			}

			//throws away the inner bounding boxes (e.g sometimes at the digit "0" the program finds the bounding box of the inner part)


			else if (((boundRect2[i].x >= boundRect2[i + 1].x) && (boundRect2[i].x - 20 <= boundRect2[i + 1].x)) ||
				((boundRect2[i].x <= boundRect2[i + 1].x) && (boundRect2[i].x >= boundRect2[i + 1].x - 20)))
			{
				if (boundRect2[i].x >= boundRect2[i + 1].x) {
					//boundRect2[i].x = boundRect2[i + 1].x;
					boundRect2[i].height += boundRect2[i + 1].height;
				}
				else if (boundRect2[i].x <= boundRect2[i + 1].x) {
					boundRect2[i].x = boundRect2[i + 1].x + 5;
					boundRect2[i].height += boundRect2[i + 1].height;
				}
				if (boundRect2[i].y > boundRect2[i + 1].y) {
					boundRect2[i].y = boundRect2[i + 1].y;
				}
				for (int j = i + 1; j < boundRect2.size() - 1; j++) {
					boundRect2[j] = boundRect2[j + 1];
				}
				boundRect2.pop_back();
				i--;
			}

			//sometimes bounding boxes are found seperately (under each other) so the above algorithm puts them together

		}
	}

	for (size_t i = 0; i < boundRect2.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		rectangle(input, boundRect2[i].tl(), boundRect2[i].br(), color, 2, 8, 0);				//Draws rectangles around the digits
	}
/*
	imshow("?", input);
	waitKey(0);
	*/
	for (size_t i = 0; i < boundRect2.size(); i++) {
		cv::Rect roi(boundRect2[i].x, boundRect2[i].y, boundRect2[i].width, boundRect2[i].height);		//we analyize each digit separetely
		ROI = input(roi);
		/*
		imshow("??", ROI);
		waitKey(0);
		*/
		number = analyzeImage(ROI, boundRect2, i);
		vysledokString += to_string(number);
		vysledokDouble = stod(vysledokString);
	}

	return vysledokDouble;
}

int DataGetter::analyzeImage(Mat roi, vector<Rect> boundRect2, int p)
{
	if (boundRect2[p].width < optimalWidth) {			//if the width of the digit is less than the optimal width and it was not thrown away, then it's definitely a 1
		return 1;
	}

	int roiRows, roiCols;
	bool jeCislo = false;

	roiRows = ROI.rows;
	roiCols = ROI.cols;

	int dWidth, dHeight, dCenter;

	dWidth = int(roiCols * 0.25);
	dHeight = int(roiRows * 0.15);
	dCenter = int(roiRows * 0.05);

	int segments[7][2][2] = { { { 0, 0 },{ boundRect2[p].width, dHeight } },	//top
	{ { 0, 0 },{ dWidth, boundRect2[p].height / 2 } },		//top-left
	{ { boundRect2[p].width - dWidth, 0 },{ boundRect2[p].width, boundRect2[p].height / 2 } },	//top-right
	{ { 0, (boundRect2[p].height / 2) - dCenter },{ boundRect2[p].width, (boundRect2[p].height / 2) + dCenter } },	//center
	{ { 0, boundRect2[p].height / 2 },{ dWidth, boundRect2[p].height } },	//bottom-left
	{ { boundRect2[p].width - dWidth - 5, boundRect2[p].height / 2 },{ boundRect2[p].width - 5, boundRect2[p].height } },	//bottom-right
	{ { 0, boundRect2[p].height - dHeight },{ boundRect2[p].width, boundRect2[p].height } }		//bottom
	};

	//we use the array above, to define our 7 segments and where are they positioned on the current digit

	int on[7];

	for (int j = 0; j < 7; j++) {
		on[j] = 0;
	}

	for (int i = 0; i < 7; i++) {			//analyzing each segments separetely
		int xA, yA, xB, yB, area, total;


		xA = segments[i][0][0];
		yA = segments[i][0][1];
		xB = segments[i][1][0];
		yB = segments[i][1][1];

		cv::Mat segROI;
		cv::Rect Roi(xA, yA, xB - xA, yB - yA);
		segROI = roi(Roi);
		cvtColor(segROI, segROI, COLOR_BGR2GRAY);

		//	imshow("?", segROI);
		//	waitKey(0);

		threshold(segROI, segROI, 58, 100, THRESH_BINARY);

		//	imshow("?", segROI);
		//	waitKey(0);

		total = cv::countNonZero(segROI);		//counts the non-zero pixels (so the pixels which are not black)

		area = (xB - xA) * (yB - yA);

		if (total / float(area) > 0.5) {		//if the number of pixels, which are not black, are bigger than the whole segment's 60%, then it's "on"
			on[i] = 1;
		}
	}

	for (int i = 0; i < 10; i++) {
		jeCislo = true;
		for (int j = 0; j < 7; j++) {
			if (Digits[i][j] == on[j]) {
				continue;
			}
			else {
				jeCislo = false;
				break;
			}
		}
		if (jeCislo) {
			return i;
		}
	}

	//compares which number it is
}


DataGetter::~DataGetter()
{
}

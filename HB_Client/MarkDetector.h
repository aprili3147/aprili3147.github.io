#pragma once

#include "opencv2/dnn.hpp"
using namespace cv;
using namespace std;

class CMarkDetector
{
private:
	CStatic* m_pStatic;
	BITMAPINFO* m_pBitmapinfo = nullptr;

	void InitBitmapInfo(int width, int height, int bpp);
	Mat PreprocessImage(const Mat& src);
	Mat SobelEdgeBinary(Mat& blur);
	Mat SortMatchedResult(const Mat& image_result, Point2d matchCenter);
	CString getSimilarityLevel(double similarity);
	void DisplayResultMat(const Mat& img);

public:
	CMarkDetector();
	~CMarkDetector();

	void SetPicCtrl(CStatic* pWnd);

	Mat matchEdgeSimple(Mat& image, Mat& temple);
	Mat matchContours(Mat& image, Mat& temple, bool useRotation);
	Mat matchByYOLO(Mat& img);
	Mat matchByTarget(Mat& img);
	Mat matchORB(Mat& image, Mat& temple);
};


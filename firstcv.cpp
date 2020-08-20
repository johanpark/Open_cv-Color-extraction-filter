#include "opencv2/opencv.hpp"
using namespace cv;
int main()
{
	Mat img;

	img = imread("C:/Users/pyhan/OneDrive/πŸ≈¡ »≠∏È/picture.jpg");
	namedWindow("Hello");
	imshow("Hello", img);
	waitKey(0);
	return 0;
}
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#define UP 2490368
#define DOWN 2621440
#define RIGHT 2555904
#define LEFT 2424832

using namespace std;
using namespace cv;

void copySubImage(Mat& src, Mat& dst, int col1, int col2) {
	if (src.size() != dst.size())
		return;
	if (col1 > col2) {
		int temp = col1;
		col1 = col2;
		col2 = col1;
	}
	if (col1 < 0) {
		col1 = 0;
	}
	if (col2 >= src.cols) {
		col2 = src.cols - 1;
	}

	src(Range(0, src.rows), Range(col1, col2 + 1)).copyTo(dst(Range(0, src.rows), Range(col1, col2 + 1)));
}

int main(int argc, const char* argv[]) {
	if (argc != 2) {
		cout << "Too few arguments, requires 2" << endl;
		return -1;
	}

	Mat image, other, blur;
	int blurSize = 25;
	bool isInfoVisible = false;

	image = imread(argv[1], IMREAD_COLOR);
	GaussianBlur(image, blur, Size(blurSize, blurSize), 0);
	other = Mat(image.size(), image.type());

	int key;
	int col = image.cols / 2;
	int delta = 1;

	do {
		copySubImage(image, other, col, image.cols);
		copySubImage(blur, other, 0, col);
		if (isInfoVisible) {
			putText(other, "Slide Velocity: " + to_string(delta), Point(10, 20), FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 0), 1);
			putText(other, "Blur Kernel Size: " + to_string(blurSize), Point(10, 35), FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 0), 1);
		}
		imshow("Gaussian Blur Visualizer", other);
		key = waitKeyEx(0);

		switch (key) {
			case LEFT:
				col = max(0, col - delta);
				break;
			case RIGHT:
				col = min(col + delta, image.cols - 1);
				break;
			case UP:
				delta = min(delta + 1, 10);
				break;
			case DOWN:
				delta = max(delta - 1, 1);
				break;
			case 'w':
			case 'W':
				blurSize = min(blurSize + 2, 101);
				GaussianBlur(image, blur, Size(blurSize, blurSize), 0);
				break;
			case 's':
			case 'S':
				blurSize = max(blurSize - 2, 1);
				GaussianBlur(image, blur, Size(blurSize, blurSize), 0);
				break;
			case 't':
			case 'T':
				isInfoVisible = !isInfoVisible;
				break;
			default:
				break;
		}
	} while (key != 27 && key != -1);
	destroyAllWindows();
	return 0;
}
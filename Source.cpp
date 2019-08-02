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

Mat image, other, blurImage;
int blurSize, key, col, delta;
bool isInfoVisible, isMouseDown;

void updateImage();
static void onMouse(int event, int x, int y, int flags, void* param);
void copySubImage(Mat&, Mat&, int, int);


bool inRange(int r, int c, int minR, int maxR, int minC, int maxC) {
	return (r >= minR && r <= maxR) && (c >= minC && c <= maxC);
}

int getInRange(int minNum, int num, int maxNum) {
	return max(minNum, max(num, minNum));
}

void updateImage() {
	copySubImage(image, other, col, image.cols);
	copySubImage(blurImage, other, 0, col);
	if (isInfoVisible) {
		putText(other, "Slide Velocity: " + to_string(delta), Point(10, 20), FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 0), 1);
		putText(other, "Blur Kernel Size: " + to_string(blurSize), Point(10, 35), FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 0), 1);
	}
	line(other, Point(col, 0), Point(col, other.rows), Scalar(255, 255, 255), 1);
	imshow("Gaussian Blur Visualizer", other);
}

static void onMouse(int event, int x, int y, int flags, void* param) {
	if (isMouseDown && !inRange(y, x, 0, image.rows - 1, 0, image.cols - 1)) {
		return;
	}
		
	switch (event)
	{
		case EVENT_LBUTTONDOWN:
			isMouseDown = true;
			col = getInRange(0, x, image.cols - 1);
			updateImage();
			break;
		case EVENT_LBUTTONUP:
			isMouseDown = false;
			break;
		case EVENT_MOUSEMOVE:
			if (isMouseDown) {
				col = getInRange(0, x, image.cols - 1);
				updateImage();
			}
		default:
			break;
	}
}

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
	namedWindow("Gaussian Blur Visualizer", WINDOW_AUTOSIZE);
	setMouseCallback("Gaussian Blur Visualizer", onMouse, 0);

	blurSize = 25;
	image = imread(argv[1], IMREAD_COLOR);
	GaussianBlur(image, blurImage, Size(blurSize, blurSize), 0);
	other = Mat(image.size(), image.type());

	col = image.cols / 2;
	delta = 1;
	isInfoVisible = false;

	do {
		updateImage();
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
				GaussianBlur(image, blurImage, Size(blurSize, blurSize), 0);
				break;
			case 's':
			case 'S':
				blurSize = max(blurSize - 2, 1);
				GaussianBlur(image, blurImage, Size(blurSize, blurSize), 0);
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
#include <unistd.h>
#include <bits/stdc++.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#define MAX_SPACE 8192
#define MAX_X 400
#define MAX_Y 400
#define SIZE 104830

using namespace cv;
using namespace std;

int main(int argc, char* argv[], char* envp[])
{	
	char bcodes_window[] = "QBCodes-Generator";
	/* Window and image titles */
	Mat bcodes_image = Mat::zeros(MAX_X, MAX_Y, CV_8UC3);
	
	/* Do processing */
	
	
	imshow(bcodes_window, bcodes_image);
	moveWindow(bcodes_window, 0, 256);
	
	/* Wait */
	waitKey(0);
	return 0;
}

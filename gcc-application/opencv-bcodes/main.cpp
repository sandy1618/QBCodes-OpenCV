#include <unistd.h>
#include <bits/stdc++.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
/* Maximum Window Area */
#define MAX_X 125
#define MAX_Y 125

/* Min Max radius of BCode */
#define min_radius 45 
#define max_radius 55
#define finite_module_size 5
#define thickness 1

/* Random numbers range */
#define MIN_RANGE 1
#define MAX_RANGE 99999999

using namespace cv;
using namespace std;

std::vector<long long int> decimals;
std::set<long long int> collision;

/* 0/1 random generator */
bool get_random()
{
	std::random_device rd;
    std::default_random_engine seed(rd());
    std::uniform_int_distribution<int> random_engine_block(MIN_RANGE, MAX_RANGE);
    auto random = std::bind(random_engine_block, seed);
    return (bool)(random() % 2);	
}

void represent_number (std::deque<bool> number)
{
	if(number.empty()) return; 
	long long int sz = number.size();
	long long int sum = 0;
	for (long long int i = 0; i < sz; i++) {
		long long int current = (number[i] ? 1 : 0);
		sum = sum << 1;
		sum += current;
	}
	decimals.push_back(sum);
}

void get_uuid (std::vector<long long int> decimals) 
{
	long long int mul = 0LL;
	mul = ((decimals.front() % (1LL << 31)) * (decimals.back() % (1LL << 31)) % (1LL << 31));
	if(collision.find(mul) != collision.end()) std::cout << "Collision Occurred" << std::endl;
		else collision.insert(mul);   
	std::cout << "Video id :  https://commonserver.aws.in/storage/../..?id=" << mul << std::endl;
}

void draw_outer_demarcation_circle(Mat bcodes_image, int current_radius)
{
	float angle_measure  = 0.0;
	bool black = 0;
	while(angle_measure <= 360) {
		
		if(black) {
			for(int i = 0; i < finite_module_size; i++) {
				ellipse (bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(current_radius - i, current_radius - i), 0, angle_measure, angle_measure + 10, Scalar(0, 0, 0), thickness, LINE_AA);
			}
			black = !(black);
		} else {
			for(int i = 0; i < finite_module_size; i++) {
				ellipse (bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(current_radius - i, current_radius - i), 0, angle_measure, angle_measure + 10, Scalar(255, 255, 255), thickness, LINE_AA);
			}
			black = !(black);
		}
		
		angle_measure += 10;
	}
}

/* This draws only only on circle */
void draw_payload_circles (Mat bcodes_image, int current_radius, float start_angle, float end_angle)
{
	float angle_measure = start_angle; // starts.
	std::deque<bool> number;
	while(angle_measure <= end_angle) {
			
			bool black = get_random(); // get random 0 or 1 value. 
			
			// Convert to decimal number later. 
			number.push_back(black);
			
			// Depending on whether 0 or 1 black or white for a given circle sector is decided. 
			if(black) {
				// Drawing it n times. Else rounded edges come up. Any better way? 
				for(int i = 0; i < finite_module_size; i++) {
					ellipse (bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(current_radius - i, current_radius - i), 0, angle_measure, angle_measure + 10, Scalar(0, 0, 0), thickness, LINE_AA);
				}
			} else {
				// Drawing it n times. Else rounded edges come up. Any better way? 
				for(int i = 0; i < finite_module_size; i++) {
					ellipse (bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(current_radius - i, current_radius - i), 0, angle_measure, angle_measure + 10, Scalar(255, 255, 255), thickness, LINE_AA);
				}
			}
						
			// Next 5 degree sector 
			angle_measure += 10;
		}
		
	represent_number(number);
}
int main(int argc, char* argv[], char* envp[])
{	
	/* Window and image titles */
	char bcodes_window[] = "QBCodes-Generator";
	
	/* 2D Grid image matrix as per OpenCV */
	Mat bcodes_image = Mat::zeros(MAX_X, MAX_Y, 0);
	
	/* Generating rings of 0/1 encoding. */
	int current_radius = max_radius;
	
	// Outer ring circle for angle identification. 
	draw_outer_demarcation_circle (bcodes_image, max_radius + finite_module_size);
	
	while(current_radius >= min_radius) {
		
		float angle_measure = 0.0;
		// Start Marker for each circle in white. From 0 to 5 degree in each circle. 
		for (int i = 0; i < finite_module_size; i++) 
			ellipse (bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(current_radius - i, current_radius - i), 0, angle_measure, angle_measure + 10, Scalar(255, 255, 255), thickness, LINE_AA );
		
		// Data Payload starts from 20 degree goes upto 340 degree. (32 bits per circle, 10 degree sectors) 
		draw_payload_circles(bcodes_image, current_radius, 20.0, 340.0);		
		
		// Next Circle in the next loop.		
		current_radius -= finite_module_size;
	}
	
	// 4 point orientation identifier. 
	for (int i = 0; i < finite_module_size; i++) 
		ellipse ( bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(40 - i, 40 - i), 0, 0, 45, Scalar(255, 255, 255), thickness, LINE_AA );
	for (int i = 0; i < finite_module_size; i++) 
		ellipse ( bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(40 - i, 40 - i), 0, 65, 135, Scalar(255, 255, 255), thickness, LINE_AA );
	for (int i = 0; i < finite_module_size; i++)
		ellipse ( bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(40 - i, 40 - i), 0, 155, 225, Scalar(255, 255, 255), thickness, LINE_AA );
	for (int i = 0; i < finite_module_size; i++) 
		ellipse ( bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(40 - i, 40 - i), 0, 245, 315, Scalar(255, 255, 255), thickness, LINE_AA );
	for (int i = 0; i < finite_module_size; i++) 
		ellipse ( bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(40 - i, 40 - i), 0, 335, 360, Scalar(255, 255, 255), thickness, LINE_AA );
	
	/* Solid White Circle at center. */
	for (int i = 0; i < finite_module_size; i++) 
		ellipse ( bcodes_image, Point(MAX_X/2, MAX_Y/2), Size(35 - i, 35 - i), 0, 0, 360, Scalar(255, 255, 255), thickness, LINE_AA );
		
	get_uuid (decimals);
	
	/* GUI Image */
	imshow(bcodes_window, bcodes_image);
	moveWindow(bcodes_window, 0, 256);
	
	/* Wait */
	waitKey(0);
	return 0;
}



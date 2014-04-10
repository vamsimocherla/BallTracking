//
//  main.cpp
//  EdgeTracking
//
//  Created by Vamsi Mocherla on 3/17/14.
//  Copyright (c) 2014 VamsiMocherla. All rights reserved.
//

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<opencv2/objdetect/objdetect.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	// capture input video from webcam
	VideoCapture input(0);
    
	if(!input.isOpened())
	{
		cout << "[ERROR] Cannot open webcam" << endl;
		exit(0);
	}
    
	cout << "[MESSAGE]: CAPTURING VIDEO FROM WEBCAM" << endl;
    
	Mat input_frame;
	Mat input_gray_frame;
    
	float oldX = 0;
	float oldY = 0;
	float newX = 0;
	float newY = 0;
	float speed = 0;

	vector<Vec3f> circles;
    
    int key = 0;
	while(key != 27)
	{
		// capture each frame of the video
		input >> input_frame;
		if(input_frame.empty())
			break;
        // resize the image for faster processing
        resize(input_frame, input_frame, Size(), 0.5, 0.5, INTER_LINEAR);
        
		// convert input frame to grayscale
		cvtColor(input_frame, input_gray_frame, CV_BGR2GRAY);
        
		// remove any salt and pepper noise by applying Median filter
		medianBlur(input_gray_frame, input_gray_frame, 5);
        
		// smooth the image by applying Gaussian filter
		GaussianBlur(input_gray_frame, input_gray_frame, Size(3,3), 3, 3);
        
		// use Hough filters to detect the ball
		HoughCircles(input_gray_frame, circles, CV_HOUGH_GRADIENT, 2, 30, 200, 100, 10, 200);
        
		Mat object;
		// Draw the circles detected
		for( size_t i = 0; i < circles.size(); i++ )
		{
            // compute the speed of the object
            newX = circles[i][0];
            newY = circles[i][1];
            speed = sqrt((newX-oldX)*(newX-oldX) + (newY-oldY)*(newY-oldY));
            cout << "Speed of the object = " << speed << " pixels per frame" << endl;

			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			float radius = cvRound(circles[i][2]);
            
			// circle center
			circle( input_frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
            
			// circle outline
			circle( input_frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
            
			// rectangle outline
			Point p1(cvRound(circles[i][0] - radius-5), cvRound(circles[i][1] - radius-5));
			Point p2(cvRound(circles[i][0] + radius+5), cvRound(circles[i][1] + radius+5));
			rectangle( input_frame, p1, p2, Scalar(255,255,255), 1, 8, 0);
		}
        
		imshow("Object", input_frame);
        
        oldX = newX;
		oldY = newY;
		key = waitKey(1);
	}
    
	// release the file capture
	input.release();
    
	return 0;
}

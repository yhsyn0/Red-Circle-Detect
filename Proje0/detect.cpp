#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture vid(0);    
    if(!vid.isOpened())
    {
        cout << "Kamera açılamadı !" << endl;
        return -1;
    }

    Mat video, HSV;

    while(1)
    {
        bool success = vid.read(video);
        if(!success)
        {
            cout << "Video akışı okunamadı !" << endl;
            return -1;
        }

        Mat original = video.clone();
        medianBlur(video, video, 3);

        cvtColor(video, HSV, COLOR_BGR2HSV);
        
        Mat up_red;
        inRange(HSV, Scalar(170, 150, 100), Scalar(179, 255, 255), up_red);

        erode(up_red, up_red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( up_red, up_red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

        dilate( up_red, up_red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(up_red, up_red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        GaussianBlur(up_red, up_red, Size(9, 9), 2, 2);

        vector<Vec3f> circles;
        HoughCircles(up_red, circles, HOUGH_GRADIENT, 1, up_red.rows/8, 100, 20, 0, 0);
        
        for(size_t current = 0; current < circles.size(); ++current)
        {
            Point center(round(circles[current][0]), round(circles[current][1]));
            int radius = round(circles[current][2]);

            circle(original, center, radius, Scalar(0, 255, 0), 5);
        }

        namedWindow("Threshold", WINDOW_AUTOSIZE);
        imshow("Threshold", up_red);
        namedWindow("Orjinal", WINDOW_AUTOSIZE);
        imshow("Orjinal", original);

        if (waitKey(30) == 27)
            break; 
    }
}
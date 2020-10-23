#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
     VideoCapture vid(0); //capture the video from web cam

     if ( !vid.isOpened() )  // if not success, exit program
     {
          cout << "Cannot open the web cam" << endl;
          return -1;
     }

     //namedWindow("Control", WINDOW_AUTOSIZE); //create a window called "Control"

     int iLowH = 170;
     int iHighH = 179;

     int iLowS = 150; 
     int iHighS = 255;

     int iLowV = 50;
     int iHighV = 255;
/*  
     //Create trackbars in "Control" window
     createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
     createTrackbar("HighH", "Control", &iHighH, 179);

     createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
     createTrackbar("HighS", "Control", &iHighS, 255);

     createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
     createTrackbar("HighV", "Control", &iHighV, 255);
*/     

     Mat video, imgHSV, imgThresholded;

     while (true)
     {
          bool bSuccess = vid.read(video); // read a new frame from video

          if (!bSuccess) //if not success, break loop
          {
               cout << "Cannot read a frame from video stream" << endl;
               break;
          }

          cvtColor(video, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

          inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

          //morphological opening (remove small objects from the foreground)
          erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
          dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))); 

          //morphological closing (fill small holes in the foreground)
          dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))); 
          erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
          
          //----------------------------------
          vector < vector < Point > > contours;
          vector < Vec4i > hierarchy;

          findContours(imgThresholded, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

          vector < Moments > mu(contours.size());

          for(int i = 0; i < contours.size(); i++)
               mu[i] = moments(contours[i]);


          vector < Point2f > mc(contours.size());

          for(int i = 0; i < contours.size(); i++)
          {    
               float mcenterx = mu[i].m10 / mu[i].m00;
               float mcentery = mu[i].m01 / mu[i].m00;
               mc[i] = Point2f((mcenterx), (mcentery));
               cout << mcenterx << " " << mcentery << endl;
               drawContours(video, contours, i, Scalar(0, 255, 0), 2, 8);
               circle(video, Point(mcenterx, mcentery), 3, Scalar(0, 255, 0), 3, LINE_AA, 0);
          }
          //----------------------------------

          imshow("Thresholded Image", imgThresholded); //show the thresholded image
          imshow("Original", video); //show the original image

          if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
               break; 
     }

     return 0;

}
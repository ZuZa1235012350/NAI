#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture cap(0); 

    if (!cap.isOpened()) 
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    namedWindow("Control"); 

    int iLowH = 95;
    int iHighH = 114;

    int iLowS = 32;
    int iHighS = 255;

    int iLowV = 58;
    int iHighV = 255;

    createTrackbar("LowH", "Control", &iLowH, 179); 
    createTrackbar("HighH", "Control", &iHighH, 179);
    createTrackbar("LowS", "Control", &iLowS, 255);
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255); 
    createTrackbar("HighV", "Control", &iHighV, 255);

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); 

        if (!bSuccess) 
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); 

        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); 
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

     
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        imshow("Thresholded Image", imgThresholded); 
        imshow("Original", imgOriginal);

        if ((waitKey(1000.0 / 60.0) & 0x0ff) == 120) 
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;

}

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    bool capturing = true;
    Mat frame;
    VideoCapture cap;
    int deviceID = 0;
    int apiID = CAP_ANY;
    cap.open(deviceID, apiID);
    if (!cap.isOpened()) {
        cerr << "error opening frames source" << endl;
        return -1;
    }
    else {
        do {
            Mat frame, frame_output;
            if (cap.read(frame)) {
                //imshow("Not-yet smart windown", frame);
                flip(frame, frame_output, 1);
                imshow("Not-yet smart windown", frame_output);
            }
            else {
                capturing = false;
            }
            if ((waitKey(1000.0 / 60.0) & 0x0ff) == 113) capturing = false;
        } while (capturing);
    }
    return 0;
}
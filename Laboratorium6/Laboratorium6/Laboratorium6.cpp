#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    bool capturing = true;
    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "error opening frames source" << endl;
        return -1;
    }
    cout << "Video size: " << cap.get(cv::CAP_PROP_FRAME_WIDTH)
        << "x" << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << endl;
    do {
        Mat frame, frame_output;
        if (cap.read(frame)) {
            //imshow("Not-yet smart windown", frame);
            flip(frame, frame_output, 1);
            imshow("Not-yet smart windown", frame_output);
        }
        else {
            // stream finished
            capturing = false;
        }
        //czekaj na klawisz, sprawdz czy to jest 'q'
        if ((cv::waitKey(1000.0 / 60.0) & 0x0ff) == 113) capturing = false;
    } while (capturing);
    return 0;
}
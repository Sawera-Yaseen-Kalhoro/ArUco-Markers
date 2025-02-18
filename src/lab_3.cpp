#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/aruco.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

int main(int argc, char* argv[]) {

    // Parse inputs
    string dictName = argv[1]; // dictionary

    // Map dictionary names to their corresponding enum values
    unordered_map<string, int> dictMap = {
        {"DICT_4X4_50", cv::aruco::DICT_4X4_50},
        {"DICT_4X4_100", cv::aruco::DICT_4X4_100},
        {"DICT_4X4_250", cv::aruco::DICT_4X4_250},
        {"DICT_4X4_1000", cv::aruco::DICT_4X4_1000},
        {"DICT_5X5_50", cv::aruco::DICT_5X5_50},
        {"DICT_5X5_100", cv::aruco::DICT_5X5_100},
        {"DICT_5X5_250", cv::aruco::DICT_5X5_250},
        {"DICT_5X5_1000", cv::aruco::DICT_5X5_1000},
        {"DICT_6X6_50", cv::aruco::DICT_6X6_50},
        {"DICT_6X6_100", cv::aruco::DICT_6X6_100},
        {"DICT_6X6_250", cv::aruco::DICT_6X6_250},
        {"DICT_6X6_1000", cv::aruco::DICT_6X6_1000},
        {"DICT_7X7_50", cv::aruco::DICT_7X7_50},
        {"DICT_7X7_100", cv::aruco::DICT_7X7_100},
        {"DICT_7X7_250", cv::aruco::DICT_7X7_250},
        {"DICT_7X7_1000", cv::aruco::DICT_7X7_1000},
        {"DICT_ARUCO_ORIGINAL", cv::aruco::DICT_ARUCO_ORIGINAL}
    };

    // Use Aruco marker
    cv::aruco::Dictionary dictionary;

    auto it = dictMap.find(dictName);
    if (it != dictMap.end()) {
        dictionary = cv::aruco::getPredefinedDictionary(it->second);
    } else {
        cerr << "Unknown dictionary name\n";
        return 1;
    }

    // Open the default camera
    cv::VideoCapture cap(0); 
    if (!cap.isOpened()) {
        cerr << "Error: Unable to open camera." << endl;
        return -1;
    }
    // Capture frames
    cv::Mat frame;
    while (cap.read(frame)) {
        if (frame.empty()) {
            cerr << "Error: Unable to read frame from camera." << endl;
            break;
        }

        // Marker Detection
        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
        cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
        cv::aruco::ArucoDetector detector(dictionary, detectorParams);
        detector.detectMarkers(frame, markerCorners, markerIds, rejectedCandidates);
        
        //Overlay Markers
        cv::aruco::drawDetectedMarkers(frame, markerCorners, markerIds);

        // Display Output
        cv::imshow("Output Window", frame); // Display output frame
        if (cv::waitKey(1) == 27) // Exit when ESC is pressed
            break;
    }

    return 0;
}

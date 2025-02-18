#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/aruco.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

int main(int argc, char* argv[]){
    // Parse inputs
    string dictName = argv[1]; // dictionary
    int markerId = atoi(argv[2]); // marker id
    int markerLength = atoi(argv[3]); // size of the marker in px
    string filename = argv[4]; // name of png file

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

    // Use the AruCo marker
    cv::aruco::Dictionary dictionary;

    auto it = dictMap.find(dictName);
    if (it != dictMap.end()) {
        dictionary = cv::aruco::getPredefinedDictionary(it->second);
    } else {
        cerr << "Unknown dictionary name\n";
        return 1;
    }

    // Initialize marker image object
    cv::Mat markerImage;

    // Generate the image
    cv::aruco::generateImageMarker(dictionary, markerId, markerLength, markerImage, 1);

    // Create a larger canvas with white frame
    int frameSize = 40; // Size of the white frame
    cv::Mat markerWithFrame(markerLength + 2 * frameSize, markerLength + 2 * frameSize, CV_8UC1, cv::Scalar(255));

    // Insert the marker into the center of the canvas (to create a white frame)
    cv::Rect roi(frameSize, frameSize, markerLength, markerLength);
    markerImage.copyTo(markerWithFrame(roi));

    // Show the image in a window
    cv::namedWindow("Out",0);
    cv::imshow("Out",markerWithFrame);

    // Save marker image to PNG
    cv::imwrite(filename, markerWithFrame);
    cout << "Marker saved as " << filename << endl;

    cv::waitKey(0);

    return 0;
}
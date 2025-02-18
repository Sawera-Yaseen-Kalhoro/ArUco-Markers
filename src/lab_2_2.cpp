#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/aruco.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

int main(int argc, char* argv[]){
    // Parse inputs
    int rows = atoi(argv[1]); // number of rows
    int columns = atoi(argv[2]); // number of columns
    string dictName = argv[3]; // dictionary
    int markerLength = atoi(argv[4]); // size of the marker in px
    int separation = atoi(argv[5]); // separation between markers in px
    string filename = argv[6]; // name of png file

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

    // Assign the AruCo marker dictionary
    cv::aruco::Dictionary dictionary;

    auto it = dictMap.find(dictName);
    if (it != dictMap.end()) {
        dictionary = cv::aruco::getPredefinedDictionary(it->second);
    } else {
        cerr << "Unknown dictionary name\n";
        return 1;
    }

    // Calculate the size of the canvas
    int boardLength = (markerLength + separation) * rows;
    int boardWidth = (markerLength + separation) * columns;

    // Create a canvas to hold the matrix of markers
    cv::Mat matrixOfMarkers(boardLength, boardWidth, CV_8UC1, cv::Scalar(255));

    // Initialize id from 0
    int markerId = 0;

    // Generate the Aruco markers
    for (int row = 0; row < rows; row++){
        for (int col = 0; col < columns; col++){
            
            // Calculate marker position on canvas
            int x = col * (markerLength + separation) + separation/2;
            int y = row * (markerLength + separation) + separation/2;

            // Generate the image
            cv::Mat markerImage;
            cv::aruco::generateImageMarker(dictionary, markerId, markerLength, markerImage, 1);

            // Insert the marker into the canvas at the correct position
            cv::Rect roi(x,y, markerLength, markerLength);
            markerImage.copyTo(matrixOfMarkers(roi));

            // Next id
            markerId++;
        }
    }

    // Show the matrix in a window
    cv::namedWindow("Out",0);
    cv::imshow("Out",matrixOfMarkers);

    // Save marker image to PNG
    cv::imwrite(filename, matrixOfMarkers);
    cout << "Marker saved as " << filename << endl;

    cv::waitKey(0);

    return 0;
}
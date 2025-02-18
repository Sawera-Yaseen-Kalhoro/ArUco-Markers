#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/aruco.hpp"
#include "opencv2/calib3d.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

int main(int argc, char* argv[]) {

    // Parsing command line arguments
    string dictName = argv[1]; // dictionary
    int markerId = atoi(argv[2]); // id of the detected marker
    double markerLength = atof(argv[3]); // length of one side of the marker in meters

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

    // Use Aruco marker dictionary
    cv::aruco::Dictionary dictionary;   
    auto it = dictMap.find(dictName);
    if (it != dictMap.end()) {
        dictionary = cv::aruco::getPredefinedDictionary(it->second);
    } else {
        cerr << "Unknown dictionary name\n";
        return 1;
    }

    // Open the default camera
    cv::VideoCapture webCam(0); 
    if (!webCam.isOpened()) {
        cerr << "Error: Unable to open camera." << endl;
        return 1;
    }

    // Initialize frames
    cv::Mat frame, frameCopy;

    // Export camera parameters
    cv::FileStorage fs("../build/camera.yaml", cv::FileStorage::READ);
    if (!fs.isOpened()) {
        cerr << "Error: Couldn't open calibration file" << endl; // throws an error if the file can't be read
        return 1;
    }

    cv::Mat cameraMatrix, distCoeffs;
    fs["cameraMatrix"] >> cameraMatrix;
    fs["distCoeffs"] >> distCoeffs;
    fs.release();

    // Define cube vertices
    vector<cv::Point3f> cubePoints;
    
    cubePoints.push_back(cv::Point3f(markerLength / 2.f, markerLength / 2.f, markerLength)); 
    cubePoints.push_back(cv::Point3f(markerLength / 2.f, -markerLength / 2.f, markerLength)); 
    cubePoints.push_back(cv::Point3f(-markerLength / 2.f, -markerLength / 2.f, markerLength)); 
    cubePoints.push_back(cv::Point3f(-markerLength / 2.f, markerLength / 2.f, markerLength)); 
    cubePoints.push_back(cv::Point3f(markerLength / 2.f, markerLength / 2.f, 0)); 
    cubePoints.push_back(cv::Point3f(markerLength / 2.f, -markerLength / 2.f, 0)); 
    cubePoints.push_back(cv::Point3f(-markerLength / 2.f, -markerLength / 2.f, 0)); 
    cubePoints.push_back(cv::Point3f(-markerLength / 2.f, markerLength / 2.f, 0)); 



    // Loop while camera is capturing frame
    while (webCam.read(frame)) {

        if (frame.empty()) {
            cerr << "Error: Unable to read frame from camera." << endl;
            break;
        }

        frame.copyTo(frameCopy);

        // Marker Detection
        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
        cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
        cv::aruco::ArucoDetector detector(dictionary, detectorParams);
        detector.detectMarkers(frame, markerCorners, markerIds, rejectedCandidates);
    
        // if at least one marker detected
        if (markerIds.size() > 0){
            // Initialize transformation objects
            size_t nMarkers = markerCorners.size();
            vector<cv::Vec3d> rvecs(nMarkers), tvecs(nMarkers);

            // Draw cube on marker
            for(int i=0; i < markerIds.size(); i++){
                if (markerIds[i] == markerId) {
                    // Estimate marker pose
                    vector<cv::Point3f> objPoints(cubePoints.end() -4,cubePoints.end());
                    cv::solvePnP(objPoints, markerCorners.at(i), cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i));

                    // Project cube vertices onto the image plane
                    vector<cv::Point2f> imagePoints;
                    cv::projectPoints(cubePoints, rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints);

                    // Draw cube edges
                    int lineThickness = 4;
                    cv::line(frameCopy, imagePoints[0], imagePoints[1], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[0], imagePoints[3], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[0], imagePoints[4], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[1], imagePoints[2], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[1], imagePoints[5], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[2], imagePoints[3], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[2], imagePoints[6], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[3], imagePoints[7], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[4], imagePoints[5], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[4], imagePoints[7], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[5], imagePoints[6], cv::Scalar(255, 0, 0), lineThickness);
                    cv::line(frameCopy, imagePoints[6], imagePoints[7], cv::Scalar(255, 0, 0), lineThickness);
                }
            }
        }

        cv::imshow("ArUco Marker Detection", frameCopy);
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    webCam.release();
    return 0;
}

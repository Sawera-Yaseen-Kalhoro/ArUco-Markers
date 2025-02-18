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

    // Parse inputs
    string dictName = argv[1]; // dictionary
    string detectorFile = argv[2]; // detector parameters file (yaml)
    int rows = atoi(argv[3]); // number of rows
    int columns = atoi(argv[4]); // number of columns
    double markerLength = atof(argv[5]); // side length of a single marker in meters
    double distance = atof(argv[6]); // distance between markers in meters
    string cameraFilename = argv[7]; // name of the calibration parameters file (yaml)

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

    // Vector to store detected marker corners and ids for each image
    vector<vector<vector<cv::Point2f>>> allMarkerCorners;
    vector<vector<int>> allMarkerIds;

    // Open the default camera
    cv::VideoCapture webCam(0); 
    if (!webCam.isOpened()) {
        cerr << "Error: Unable to open camera." << endl;
        return -1;
    }

    cv::Mat frame; // Initialize camera frame (with overlay)
    cv::Mat frameWithoutOverlay; // Initialize image frame without overlay (for saving)
    char keyPressed = 0; // Initialize key pressed variable
    int imgId = 0; // Initialize variable to store the number of images captured

    // Vector to store image points for each image
    vector<vector<cv::Point2f>> imagePointsPerImage;

    while (webCam.isOpened()) { 
                
        // Capture frames
        while (webCam.read(frame)) {
            if (frame.empty()) {
                cerr << "Error: Unable to read frame from camera." << endl;
                break;
            }

            // Copy the frame to save img without overlay
            frameWithoutOverlay = frame.clone();


            // Marker Detection
            std::vector<int> markerIds;
            std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
            cv::aruco::DetectorParameters detectorParams;
            if (detectorFile == "None"){
                detectorParams = cv::aruco::DetectorParameters(); // Use default detector parameter if not specified
            }
            cv::aruco::ArucoDetector detector(dictionary, detectorParams);
            detector.detectMarkers(frame, markerCorners, markerIds, rejectedCandidates);
            
            //Overlay Markers
            cv::aruco::drawDetectedMarkers(frame, markerCorners, markerIds);


            // Display Output
            cv::namedWindow("Output Window", cv::WINDOW_AUTOSIZE);
            cv::imshow("Output Window", frame); // Display output frame
            keyPressed = cv::waitKey(1);

        
            // Capture the image when 'c' is pressed
            if (keyPressed == 99) {
                // Save marker image to PNG
                imgId++;
                string imgFilename = "image" + to_string(imgId) + ".png";
                cv::imwrite(imgFilename, frameWithoutOverlay);
                
                // Store detected marker corners and ids
                allMarkerCorners.push_back(markerCorners);
                allMarkerIds.push_back(markerIds);
                cout << "Image " << imgId << " saved." << endl;
            }

            // Break loop if ESC key is pressed
            if (keyPressed == 27) {
                break;
            }
        }

        // Break outer loop if ESC key is pressed
        if (keyPressed == 27) {
            break;
        }
    }

    // Close camera
    webCam.release();
    cv::destroyAllWindows();

    // Prepare data for calibration
    vector<cv::Point3f> objectPoints;
    vector<cv::Point2f> imagePoints;
    vector<cv::Mat> processedObjectPoints, processedImagePoints;
    size_t nFrames = allMarkerCorners.size();

    // Create board object
    cv::aruco::GridBoard gridboard(cv::Size(columns, rows), markerLength, distance, dictionary);

    // Pre-process image points and object points for every frame
    for(size_t frame = 0; frame < nFrames; frame++) {
        cv::Mat currentImgPoints, currentObjPoints;

        // Match object points with image points (using the gridboard)
        gridboard.matchImagePoints(
            allMarkerCorners[frame], allMarkerIds[frame],
            currentObjPoints, currentImgPoints
        );

        // Store the pre-processed image points and object points
        if(currentImgPoints.total() > 0 && currentObjPoints.total() > 0) {
            processedImagePoints.push_back(currentImgPoints);
            processedObjectPoints.push_back(currentObjPoints);
        }
    }

    // Perform camera calibration
    cv::Size imgSize = frameWithoutOverlay.size();
    cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F), distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;

    double repError = cv::calibrateCamera(processedObjectPoints, processedImagePoints, imgSize, cameraMatrix, distCoeffs, rvecs, tvecs);

    // Save calibration parameters
    cv::FileStorage fs(cameraFilename, cv::FileStorage::WRITE);
    fs << "cameraMatrix" << cameraMatrix;
    fs << "distCoeffs" << distCoeffs;
    fs << "repError" << repError;
    fs.release();

    cout << "Calibration done. Reprojection error: " << repError << endl;

    return 0;
    
}
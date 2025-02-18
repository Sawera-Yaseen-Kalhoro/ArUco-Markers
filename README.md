# Hands-On Perception -- Lab 1: ArUco Markers

Sawera Yaseen & Vania Katherine Mulia

## Overview of the Package

The contents of this package are:

* `build` folder contains executables and program outputs such as marker image (from lab 2), board image (from lab 2), and a YAML file containing camera parameters from calibration (from lab 4). Further explanations for each executable, including how to run it, can be found in the following sections. **Note:** In case the executables cannot run at the first time, try re-building them using CMake in this folder.

* `src` folder contains the source files (`.cpp` files) for all programs in this lab.

* `images` folder contains the dataset that we captured during camera calibration to generate the camera parameters in `camera.yaml` file that can be found in the `build` folder. (This YAML file can also be seen in the report.)

## Lab 2: Generation of ArUco Markers

### Part 1: Generate 1 Marker

Generate a single marker. The source code for this program can be seen in `src/lab_2_1.cpp`. To run this program, run in the command line interface in the following format:

```bash
./generate_marker dictName markerId markerLengthPx markerFilename
```

Example:

```bash
./generate_marker DICT_ARUCO_ORIGINAL 25 200 marker.png
```

### Part 2: Generate a Board of Markers

Generate a board of markers. The source code for this program can be seen in `src/lab_2_2.cpp`. To run this program, run in the command line interface in the following format:

```bash
./generate_board rows columns dictName markerLengthPx separationPx boardFilename
```

Example:

```bash
./generate_board 2 4 DICT_ARUCO_ORIGINAL 200 80 board.png
```

## Lab 3: Detection of ArUco Markers

Detect ArUco markers. The source code for this program can be seen in `src/lab_3.cpp`. To run this program, run in the command line interface in the following format:

```bash
./detect_marker dictName
```

Example:

```bash
./detect_marker DICT_ARUCO_ORIGINAL
```

## Lab 4: Camera Calibration with ArUco Markers

Calibrate camera using ArUco board. The source code for this program can be seen in `src/lab_4.cpp`. To run this program, run in the command line interface in the following format:

```bash
./camera_calibration dictName detectorFile rows columns markerLengthMeter separationMeter cameraFilename
```

Example:

```bash
./camera_calibration DICT_ARUCO_ORIGINAL None 2 4 0.05 0.02 testing.yaml
```

**Note:** While the camera is running, press 'c' to capture the image for calibration, and press 'ESC' to close the camera and start the calibration.

## Lab 5: Augmented Reality Using ArUco Markers
### Part 1: Pose Estimation

Estimate pose of a single ArUco marker and draw the axes on the marker. The source code for this program can be seen in `src/lab_5_1.cpp`. To run this program, run in the command line interface in the following format:

```bash
./pose_estimation dictName markerId markerLengthMeter
```

Example:

```bash
./pose_estimation DICT_ARUCO_ORIGINAL 25 0.048
```

**Note:** This program uses camera parameters in a YAML file. By default, it uses `camera.yaml` file in the `build` folder. To change the camera parameters, change the path to the YAML file in the source code, in line 61.

### Part 2: Augmented Reality

Draw a cube on a single ArUco marker. The source code for this program can be seen in `src/lab_5_2.cpp`. To run this program, run in the command line interface in the following format:

```bash
./draw_cube dictName markerId markerLengthMeter
```

Example:

```bash
./draw_cube DICT_ARUCO_ORIGINAL 25 0.048
```

**Note:** This program uses camera parameters in a YAML file. By default, it uses `camera.yaml` file in the `build` folder. To change the camera parameters, change the path to the YAML file in the source code, in line 61.
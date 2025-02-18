cmake_minimum_required(VERSION 3.1)
project(lab1_aruco_markers)

set (CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)

include_directories(${OPENCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${OpenCV_LIBRARY_DIRS})

# Executable for lab 2 part 1
set(lab2_1_src
    src/lab_2_1.cpp
   )
add_executable(generate_marker ${lab2_1_src})
target_link_libraries(generate_marker
    ${OpenCV_LIBRARIES}
    )

target_compile_options(generate_marker
    PRIVATE -O3 -std=c++11
    )

# Executable for lab 2 part 2
set(lab2_2_src
    src/lab_2_2.cpp
   )
add_executable(generate_board ${lab2_2_src})
target_link_libraries(generate_board
    ${OpenCV_LIBRARIES}
    )

target_compile_options(generate_board
    PRIVATE -O3 -std=c++11
    )

# Executable for lab 3
set(lab3_src
    src/lab_3.cpp
   )
add_executable(detect_marker ${lab3_src})
target_link_libraries(detect_marker
    ${OpenCV_LIBRARIES}
    )

target_compile_options(detect_marker
    PRIVATE -O3 -std=c++11
    )

# Executable for lab 4
set(lab4_src
    src/lab_4.cpp
   )
add_executable(camera_calibration ${lab4_src})
target_link_libraries(camera_calibration
    ${OpenCV_LIBRARIES}
    )

target_compile_options(camera_calibration
    PRIVATE -O3 -std=c++11
    )

# Executable for lab 5 part 1
set(lab5_1_src
    src/lab_5_1.cpp
   )
add_executable(pose_estimation ${lab5_1_src})
target_link_libraries(pose_estimation
    ${OpenCV_LIBRARIES}
    )

target_compile_options(pose_estimation
    PRIVATE -O3 -std=c++11
    )

# Executable for lab 5 part 2
set(lab5_2_src
    src/lab_5_2.cpp
   )
add_executable(draw_cube ${lab5_2_src})
target_link_libraries(draw_cube
    ${OpenCV_LIBRARIES}
    )

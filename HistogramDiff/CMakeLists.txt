project(histogramdiff_example)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

find_package(MetavisionSDK COMPONENTS core driver REQUIRED)

find_package(OpenCV COMPONENTS)

find_package(Eigen3 CONFIG REQUIRED)

set (sample histogramdiff_example)

set (library histogramdiff)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/Release)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/Release)

add_executable(${sample} ${sample}.cpp ${library}.h)

target_link_libraries(${sample} MetavisionSDK::core MetavisionSDK::driver opencv_highgui Eigen3::Eigen)

cmake_minimum_required(VERSION 3.16)
project(image_process VERSION 0.1 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)

find_package(OpenCV 4 REQUIRED)
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    "include/"
    "third_party/onnxruntime/include"
    )
file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

add_library(image_process STATIC ${HEADERS} ${SOURCES})

target_link_libraries(image_process
    PRIVATE
    ${OpenCV_LIBS}
    ${PROJECT_SOURCE_DIR}/third_party/onnxruntime/lib/libonnxruntime.so)

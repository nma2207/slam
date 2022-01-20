cmake_minimum_required(VERSION 3.16)

project(slam VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Multimedia MultimediaWidgets REQUIRED)
find_package(OpenCV 4 REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    "include/"
    )
file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

qt_add_executable(appslam
    main.cpp
    ${HEADERS} ${SOURCES})

qt_add_qml_module(appslam
    URI slam
    VERSION 1.0
    QML_FILES main.qml 
)

target_compile_definitions(appslam
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appslam
    PRIVATE Qt6::Quick
    Qt6::Multimedia
    Qt::MultimediaWidgets
    ${OpenCV_LIBS}
    )

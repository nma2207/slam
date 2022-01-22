cmake_minimum_required(VERSION 3.16)

project(app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Multimedia MultimediaWidgets REQUIRED)
find_package(OpenCV 4 REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    "include/"
    "../image_process/include"
    )
file(GLOB HEADERS include/*.h)
file(GLOB SOURCES main.cpp src/*.cpp)

qt_add_executable(app
    main.cpp
    ${HEADERS} ${SOURCES})
#qt_add_library(app STATIC ${HEADERS} ${SOURCES})
qt_add_qml_module(app
    URI slam
    VERSION 1.0
    QML_FILES qml/main.qml
)


target_compile_definitions(app
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(app
    PRIVATE Qt6::Quick
    Qt6::Multimedia
    Qt::MultimediaWidgets
    ${OpenCV_LIBS}
    image_process
    )

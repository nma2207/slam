cmake_minimum_required(VERSION 3.16)

project(app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 17)

find_package(Qt6 6.2 COMPONENTS Quick Core Gui REQUIRED)
find_package(OpenCV 4 REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    "include/"
    "../image_process/include"
    )

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)
file(GLOB RESOURCES)

qt_add_resources(RESOURCES resource/qml.qrc)
qt_add_executable(app
    main.cpp
    ${HEADERS} ${SOURCES} ${RESOURCES})

target_compile_definitions(app
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(app
    PRIVATE Qt6::Quick
    Qt6::Core
    Qt6::Gui
    ${OpenCV_LIBS}
    image_process
    )

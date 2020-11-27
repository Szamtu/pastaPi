cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(tkdnn_yolo VERSION ${PastaPi_VERSION} LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(OpenCV REQUIRED)
include_directories("/opt/cuda/include")

set(SOURCES
    dnn_tkdnn/dnn_tkdnn.cc
    dnn_tkdnn/dnn_tkdnn.h

    tkdnn_yolo.cc
)

set(GUI_SOURCES
    dnn_tkdnn/dnn_tkdnn_node.cc
    dnn_tkdnn/dnn_tkdnn_node.h
    )

set(LIB_GUI
    ${OpenCV_LIBS}
	tkDNN
    cudart
    )

set(LIB_NO_GUI
    ${OpenCV_LIBS}
	tkDNN
    cudart
    )

include(PluginBuild)

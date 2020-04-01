cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(OpenCV VERSION ${PastaPi_VERSION} LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(OpenCV REQUIRED)

set(SOURCES
    videoio/video_capture/video_capture.cc
    videoio/video_capture/video_capture.h
    videoio/video_writer/video_writer.cc
    videoio/video_writer/video_writer.h

    ui/video_display/video_display.cc
    ui/video_display/video_display.h
    ui/draw_contours/draw_contours.cc
    ui/draw_contours/draw_contours.h

    video/mog2/mog2.cc
    video/mog2/mog2.h
    video/video_bufer/video_bufer.cc
    video/video_bufer/video_bufer.h

    imcodecs/imwrite/imwrite.cc
    imcodecs/imwrite/imwrite.h

    improc/color2gray/color2gray.cc
    improc/color2gray/color2gray.h
    improc/crop_image/crop_image.cc
    improc/crop_image/crop_image.h
    improc/find_conturs/findcontours.cc
    improc/find_conturs/findcontours.h
    improc/medianblur/medianblur.cc
    improc/medianblur/medianblur.h
    improc/resize/resize.cc
    improc/resize/resize.h

    dnn/yolo/yolo.cc
    dnn/yolo/yolo.h
    dnn/dnn_threshold/dnn_threshold.cc
    dnn/dnn_threshold/dnn_threshold.h
    dnn/read_classnames/read_classnames.cc
    dnn/read_classnames/read_classnames.h
    dnn/dnn_draw_labels/dnn_draw_labels.cc
    dnn/dnn_draw_labels/dnn_draw_labels.h
    dnn/find_class/find_class.cc
    dnn/find_class/find_class.h
    dnn/extract_rect/extract_rect.cc
    dnn/extract_rect/extract_rect.h

    opencv.cc
)

set(GUI_SOURCES
    ui/video_display/video_display_node.cc
    ui/video_display/video_display_node.h
    dnn/yolo/yolo_node.cc
    dnn/yolo/yolo_node.h
    )

set(LIB_GUI
    ${OpenCV_LIBS}
    )

set(LIB_NO_GUI
    ${OpenCV_LIBS}
    )

include(PluginBuild)

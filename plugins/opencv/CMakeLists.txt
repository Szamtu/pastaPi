cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(OpenCV VERSION 17.09.06 LANGUAGES C CXX)

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

    opencv.cc
)

set(GUI_SOURCES
    ui/video_display/video_display_node.cc
    ui/video_display/video_display_node.h
    )

if (BUILD_PLUGINS_GUI)
    set(OPENCV_SOURCES ${SOURCES} ${GUI_SOURCES})
else()
    set(OPENCV_SOURCES ${SOURCES})
endif ()

add_library(${PROJECT_NAME} MODULE ${OPENCV_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_ROOT})
target_compile_definitions(${PROJECT_NAME}
  PUBLIC SPAGHETTI_SHARED
  PRIVATE SPAGHETTI_EXPORTS ${SPAGHETTI_DEFINITIONS}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_DEFINITIONS_DEBUG}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_DEFINITIONS_RELEASE}>
  )
target_compile_options(${PROJECT_NAME}
  PRIVATE ${SPAGHETTI_FLAGS}
  PRIVATE ${SPAGHETTI_FLAGS_C}
  PRIVATE ${SPAGHETTI_FLAGS_CXX}
  PRIVATE ${SPAGHETTI_FLAGS_LINKER}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_FLAGS_DEBUG}>
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_WARNINGS}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_FLAGS_RELEASE}>
  )
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")

if (BUILD_PLUGINS_GUI)
  add_compile_definitions(BUILD_PLUGIN_GUI)
  target_link_libraries(${PROJECT_NAME} Spaghetti SpaghettiUI Qt5::Core ${OpenCV_LIBS})
else()
  target_link_libraries(${PROJECT_NAME} Spaghetti Qt5::Core ${OpenCV_LIBS})
endif ()

install(TARGETS ${PROJECT_NAME}
  COMPONENT ${PROJECT_NAME}Plugin
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/spaghetti
  )

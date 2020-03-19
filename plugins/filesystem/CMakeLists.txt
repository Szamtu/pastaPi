cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(Filesystem VERSION ${PastaPi_VERSION} LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Core REQUIRED)

set(SOURCES
    dir_select/dir_select.cc
    dir_select/dir_select.h
    dir_list/dir_list.cc
    dir_list/dir_list.h

    filesystem.cc
    )
   
set(GUI_SOURCES
    dir_select/dir_select_node.cc
    dir_select/dir_select_node.h
    dir_list/dir_list_node.cc
    dir_list/dir_list_node.h
   )
   
include(PluginBuild)

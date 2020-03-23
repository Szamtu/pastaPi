cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(LogicCounters VERSION 17.09.06 LANGUAGES C CXX)

set(SOURCES
    count_up/count_up.cc
    count_up/count_up.h

    counters.cc
)

include(PluginBuild)

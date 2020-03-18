cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(ExamplePlugin VERSION ${PastaPi_VERSION} LANGUAGES C CXX)

set(SOURCES
  example.cc
  )

include(PluginBuild)

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(PastaPi
  VERSION 00.11.00
  DESCRIPTION "Graphical openCV image processor"
  LANGUAGES C CXX
)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(DefaultOptions)
include(BuildDirectories)
include(BuildType)
include(CompilerType)
include(CompilerFlags)
include(GetRepoInfo)
include(VendorHeaders)

option(PASTAPI_BUILD_EDITOR "Build editor" ON)
option(PASTAPI_BUILD_PLUGINS "Build plugins" ON)
option(PASTAPI_BUILD_EXAMPLE_PLUGIN "Build example plugin" ON)
option(SPAGHETTI_ENABLE_ALL_WARNINGS "Enable all warnings" OFF)
option(SPAGHETTI_TREAT_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(SPAGHETTI_USE_OPENGL "Use OpenGL in QGraphicsView" OFF)
option(BUILD_SHARED_LIBS "Build shared libs" ON)

set(VENDOR_ROOT ${PROJECT_SOURCE_DIR}/vendor)

add_subdirectory(libspaghetti)

if (PASTAPI_BUILD_EDITOR)
  add_subdirectory(editor)
  add_subdirectory(libspaghettiui)
  set(BUILD_PLUGINS_GUI ON)
endif ()

if (PASTAPI_BUILD_PLUGINS)
  add_subdirectory(plugins)
endif ()

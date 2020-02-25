cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(libSpaghetti
    VERSION ${Spaghetti_VERSION}
    LANGUAGES C CXX
)

find_package(Threads REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
if (SPAGHETTI_USE_OPENGL)
  find_package(Qt5 REQUIRED COMPONENTS OpenGL)
endif ()
if (SPAGHETTI_USE_CHARTS)
  find_package(Qt5 REQUIRED COMPONENTS Charts)
endif ()

find_package(OpenCV REQUIRED)

if (HAVE_CXX_FILESYSTEM)
  set(SPAGHETTI_FILESYSTEM_IMPLEMENTATION SPAGHETTI_FS_STD_FILESYSTEM)
elseif (HAVE_CXX_EXPERIMENTAL_FILESYSTEM)
  set(SPAGHETTI_FILESYSTEM_IMPLEMENTATION SPAGHETTI_FS_STD_EXPERIMENTAL_FILESYSTEM)
elseif (HAVE_BOOST_FILESYSTEM)
  set(SPAGHETTI_FILESYSTEM_IMPLEMENTATION SPAGHETTI_FS_BOOST_FILESYSTEM)
endif ()

configure_file(source/filesystem.h.in include/filesystem.h)
configure_file(include/spaghetti/version.h.in include/spaghetti/version.h)

set(LIBSPAGHETTI_PUBLIC_GATES_HEADERS
  include/spaghetti/elements/gates/all.h
  )
set(LIBSPAGHETTI_PUBLIC_LOGIC_HEADERS
  include/spaghetti/elements/logic/all.h
  )
set(LIBSPAGHETTI_PUBLIC_MATH_HEADERS
  include/spaghetti/elements/math/all.h
  )
set(LIBSPAGHETTI_PUBLIC_TIMERS_HEADERS
  include/spaghetti/elements/timers/all.h
  )
set(LIBSPAGHETTI_PUBLIC_UI_HEADERS
  include/spaghetti/elements/ui/all.h
  )
set(LIBSPAGHETTI_PUBLIC_VALUES_HEADERS
  include/spaghetti/elements/values/all.h
  )
set(LIBSPAGHETTI_PUBLIC_OPENCV_HEADERS
  include/spaghetti/elements/opencv/all.h
  )
set(LIBSPAGHETTI_PUBLIC_ELEMENTS_ALL_HEADERS
  include/spaghetti/elements/all.h
  )
set(LIBSPAGHETTI_PUBLIC_COMMON_HEADERS
  include/spaghetti/api.h
  include/spaghetti/editor.h
  include/spaghetti/sockvalues.h
  include/spaghetti/element.h
  include/spaghetti/logger.h
  include/spaghetti/node.h
  include/spaghetti/package.h
  include/spaghetti/registry.h
  include/spaghetti/socket_item.h
  include/spaghetti/strings.h
  include/spaghetti/utils.h
  )

set(LIBSPAGHETTI_PUBLIC_HEADERS
  ${LIBSPAGHETTI_PUBLIC_GATES_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_LOGIC_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_MATH_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_PNEUMATIC_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_TIMERS_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_UI_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_VALUES_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_OPENCV_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_ELEMENTS_ALL_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_COMMON_HEADERS}
  )

set(LIBSPAGHETTI_GENERATED_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/version.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/filesystem.h
  )

set(LIBSPAGHETTI_SOURCES
  ${LIBSPAGHETTI_PUBLIC_HEADERS}
  include/spaghetti/version.h.in

  source/icons/icons.qrc

  source/nodes/logic/all.h

  source/nodes/ui/all.h

  source/nodes/values/all.h

  source/nodes/opencv/all.h

  source/nodes/all.h
  source/nodes/package.cc
  source/nodes/package.h

  source/ui/colors.cc
  source/ui/colors.h
  source/ui/editor.cc
  source/ui/editor.ui
  source/ui/elementstree.cc
  source/ui/elementstree.h
  source/ui/link_item.cc
  source/ui/link_item.h
  source/ui/package_view.cc
  source/ui/package_view.h
  source/ui/socket_item.cc

  source/sockvalues.cc
  source/element.cc
  source/logger.cc
  source/node.cc
  source/package.cc
  source/registry.cc
  source/shared_library.cc
  source/shared_library.h
  source/filesystem.h.in
  )

set(LIBSPAGHETTI_CHARTS_SOURCES
  source/nodes/values/characteristic_curve/editor_widget.cc
  source/nodes/values/characteristic_curve/editor_widget.h
  source/nodes/values/characteristic_curve/editor_window.cc
  source/nodes/values/characteristic_curve/editor_window.h
  source/nodes/values/characteristic_curve/editor_window.ui
  source/nodes/values/characteristic_curve/generators.cc
  source/nodes/values/characteristic_curve/generators.h
  source/nodes/values/characteristic_curve/line.cc
  source/nodes/values/characteristic_curve/line.h
  source/nodes/values/characteristic_curve/point.cc
  source/nodes/values/characteristic_curve/point.h
  source/nodes/values/characteristic_curve.cc
  source/nodes/values/characteristic_curve.h
  )

set(LIBSPAGHETTI_ALL_SOURCES
  ${LIBSPAGHETTI_GENERATED_SOURCES}
  ${LIBSPAGHETTI_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIBSPAGHETTI_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIBSPAGHETTI_CHARTS_SOURCES})
source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${LIBSPAGHETTI_GENERATED_SOURCES})

add_library(Spaghetti SHARED ${LIBSPAGHETTI_ALL_SOURCES})
target_sources(Spaghetti PRIVATE $<$<BOOL:${SPAGHETTI_USE_CHARTS}>:${LIBSPAGHETTI_CHARTS_SOURCES}>)
target_compile_features(Spaghetti PUBLIC cxx_std_17)
target_compile_definitions(Spaghetti
  PUBLIC SPAGHETTI_SHARED
  PRIVATE SPAGHETTI_EXPORTS ${SPAGHETTI_DEFINITIONS}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_DEFINITIONS_DEBUG}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_DEFINITIONS_RELEASE}>
  PRIVATE $<$<BOOL:${SPAGHETTI_USE_OPENGL}>:SPAGHETTI_USE_OPENGL>
  PRIVATE $<$<BOOL:${SPAGHETTI_USE_CHARTS}>:SPAGHETTI_USE_CHARTS>
  )
target_compile_options(Spaghetti
  PRIVATE ${SPAGHETTI_FLAGS}
  PRIVATE ${SPAGHETTI_FLAGS_C}
  PRIVATE ${SPAGHETTI_FLAGS_CXX}
  PRIVATE ${SPAGHETTI_FLAGS_LINKER}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_FLAGS_DEBUG}>
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_WARNINGS}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_FLAGS_RELEASE}>
  )
target_include_directories(Spaghetti
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  PRIVATE source
  )
target_include_directories(Spaghetti SYSTEM PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/include
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/spdlog/include>
  )
target_link_libraries(Spaghetti
  PUBLIC ${CMAKE_THREAD_LIBS_INIT} Qt5::Widgets ${OpenCV_LIBS}
  PRIVATE ${CMAKE_DL_LIBS} ${CXX_FILESYSTEM_LIBS}
  PRIVATE $<$<BOOL:${SPAGHETTI_USE_OPENGL}>:Qt5::OpenGL>
  PRIVATE $<$<BOOL:${SPAGHETTI_USE_CHARTS}>:Qt5::Charts>
)
if (CLANG)
  target_link_libraries(Spaghetti PUBLIC -stdlib=libc++)
endif ()

install(TARGETS Spaghetti
  COMPONENT SDK
  EXPORT SpaghettiConfig
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
install(FILES ${LIBSPAGHETTI_PUBLIC_GATES_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/gates)
install(FILES ${LIBSPAGHETTI_PUBLIC_LOGIC_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/logic)
install(FILES ${LIBSPAGHETTI_PUBLIC_MATH_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/math)
install(FILES ${LIBSPAGHETTI_PUBLIC_PNEUMATIC_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/pneumatic)
install(FILES ${LIBSPAGHETTI_PUBLIC_TIMERS_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/timers)
install(FILES ${LIBSPAGHETTI_PUBLIC_UI_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/ui)
install(FILES ${LIBSPAGHETTI_PUBLIC_VALUES_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/values)
install(FILES ${LIBSPAGHETTI_PUBLIC_OPENCV_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/opencv)
install(FILES ${LIBSPAGHETTI_PUBLIC_ELEMENTS_ALL_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements)
install(FILES ${LIBSPAGHETTI_PUBLIC_COMMON_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/version.h
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti)
install(EXPORT SpaghettiConfig
  COMPONENT SDK
  DESTINATION share/Spaghetti/cmake)
export(TARGETS Spaghetti FILE SpaghettiConfig.cmake)

# Copy vendor headers to libspaghetti/include
message (STATUS "Synchronizing vendor headers...")
foreach(HEADER ${VENDOR_FREE_STANDING_HEADERS})
  configure_file(${VENDOR_ROOT}/${HEADER} ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/vendor/${HEADER} COPYONLY)
  install(FILES ${VENDOR_ROOT}/${HEADER}
    COMPONENT SDK
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/vendor)
endforeach ()
foreach(HEADER ${VENDOR_SPARSEPP_HEADERS})
  configure_file(${VENDOR_ROOT}/sparsepp/${HEADER} ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/vendor/sparsepp/${HEADER} COPYONLY)
endforeach ()
install(DIRECTORY ${VENDOR_ROOT}/sparsepp
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/vendor)
foreach(HEADER ${VENDOR_SPDLOG_HEADERS})
  configure_file(${VENDOR_ROOT}/spdlog/include/spdlog/${HEADER} ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/vendor/spdlog/${HEADER} COPYONLY)
endforeach ()
install(DIRECTORY ${VENDOR_ROOT}/spdlog/include/spdlog
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/vendor)

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(libSpaghettiUI
    VERSION ${PastaPi_VERSION}
    LANGUAGES C CXX
)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(LIBSPAGHETTIUI_PUBLIC_COMMON_HEADERS
  include/spaghettiui/api_ui.h
  include/spaghettiui/node.h
  include/spaghettiui/colors.h
  include/spaghettiui/package_view.h
  include/spaghettiui/socket_item.h
  include/spaghettiui/link_item.h
  include/spaghettiui/package.h
  include/spaghettiui/editor_package.h
  )

set(LIBSPAGHETTIUI_SOURCES
  ${LIBSPAGHETTIUI_PUBLIC_COMMON_HEADERS}

  source/node.cc
  source/colors.cc
  source/package_view.cc
  source/socket_item.cc
  source/link_item.cc
  source/package.cc
  )

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIBSPAGHETTIUI_SOURCES})

add_library(SpaghettiUI SHARED ${LIBSPAGHETTIUI_SOURCES})
target_compile_features(SpaghettiUI PUBLIC cxx_std_17)
target_compile_definitions(SpaghettiUI
  PUBLIC SPAGHETTIUI_SHARED
  PRIVATE SPAGHETTIUI_EXPORTS ${SPAGHETTI_DEFINITIONS}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_DEFINITIONS_DEBUG}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_DEFINITIONS_RELEASE}>
  )
target_compile_options(SpaghettiUI
  PRIVATE ${SPAGHETTI_FLAGS}
  PRIVATE ${SPAGHETTI_FLAGS_C}
  PRIVATE ${SPAGHETTI_FLAGS_CXX}
  PRIVATE ${SPAGHETTI_FLAGS_LINKER}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_FLAGS_DEBUG}>
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_WARNINGS}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_FLAGS_RELEASE}>
  )
target_include_directories(SpaghettiUI
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  PRIVATE source
  )
target_include_directories(SpaghettiUI SYSTEM PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/include
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/spdlog/include>
  )

target_link_libraries(SpaghettiUI
  PUBLIC ${CMAKE_THREAD_LIBS_INIT} Spaghetti Qt5::Widgets
)

install(TARGETS SpaghettiUI
  COMPONENT SDK
  EXPORT SpaghettiUIConfig
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

install(FILES ${LIBSPAGHETTIUI_PUBLIC_COMMON_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghettiIU)
install(EXPORT SpaghettiUIConfig
  COMPONENT SDK
  DESTINATION share/SpaghettiUI/cmake)
export(TARGETS SpaghettiUI FILE SpaghettiIUConfig.cmake)

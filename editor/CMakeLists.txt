cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(PastaPiEditor VERSION ${PastaPi_VERSION} LANGUAGES C CXX)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(UI
  ui/aboutpastapi.cc
  ui/aboutpastapi.h
  ui/aboutpastapi.ui
  ui/colors.cc
  ui/colors.h
  ui/editor.cc
  ui/editor.h
  ui/editor.ui
  ui/elementstree.cc
  ui/elementstree.h
  ui/link_item.cc
  ui/link_item.h
  ui/package_view.cc
  ui/package_view.h
  ui/socket_item.cc
  ui/socket_item.h
)

set(SOURCES
  source/main.cc
  source/package.cc
  source/package.h
  source/node.cc
  source/node.h
)

add_executable(${PROJECT_NAME} ${UI} ${SOURCES})
target_compile_definitions(${PROJECT_NAME}
  PRIVATE ${SPAGHETTI_DEFINITIONS}
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
target_link_libraries(${PROJECT_NAME} Spaghetti Qt5::Widgets)

install(TARGETS ${PROJECT_NAME}
  COMPONENT Editor
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

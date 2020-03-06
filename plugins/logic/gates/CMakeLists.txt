cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(LogicGates VERSION 17.09.06 LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    and/and.cc
    and/and.h

    nand/nand.cc
    nand/nand.h

    or/or.cc
    or/or.h

    nor/nor.cc
    nor/nor.h

    not/not.cc
    not/not.h

    gates.cc
)

add_library(${PROJECT_NAME} MODULE ${SOURCES})
target_include_directories(Example PRIVATE ${PROJECT_ROOT})
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
set_target_properties(Example PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
set_target_properties(Example PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")

target_link_libraries(${PROJECT_NAME} Spaghetti)

install(TARGETS ${PROJECT_NAME}
  COMPONENT ${PROJECT_NAME}Plugin
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/spaghetti
  )

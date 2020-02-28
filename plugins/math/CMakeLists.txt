cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(MathPlugin VERSION 17.09.06 LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    arithmetic/subtraction/subtraction.cc
    arithmetic/subtraction/subtraction.h

    arithmetic/addition/addition.cc
    arithmetic/addition/addition.h

    arithmetic/multiplication/multiplication.cc
    arithmetic/multiplication/multiplication.h

    arithmetic/division/division.cc
    arithmetic/division/division.h

    arithmetic/power/power.cc
    arithmetic/power/power.h

    arithmetic/square/square.cc
    arithmetic/square/square.h

    arithmetic/modulo/modulo.cc
    arithmetic/modulo/modulo.h

    arithmetic/absolute/absolute.cc
    arithmetic/absolute/absolute.h

    trigonometry/sinus/sinus.cc
    trigonometry/sinus/sinus.h

    trigonometry/cosinus/cosinus.cc
    trigonometry/cosinus/cosinus.h

    trigonometry/tangens/tangens.cc
    trigonometry/tangens/tangens.h

    rounding/floor/floor.cc
    rounding/floor/floor.h

    math.cc
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

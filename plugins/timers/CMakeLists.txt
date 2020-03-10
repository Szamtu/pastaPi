cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(Timers VERSION 17.09.06 LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    clock/clock.cc
    clock/clock.h

    delta_time/delta_time.cc
    delta_time/delta_time.h

    t_off/t_off.cc
    t_off/t_off.h

    t_on/t_on.cc
    t_on/t_on.h

    t_pulse/t_pulse.cc
    t_pulse/t_pulse.h

    timers.cc
)

set(GUI_SOURCES
    clock/clock_node.cc
    clock/clock_node.h
    )

if (BUILD_PLUGINS_GUI)
    set(TIMERS_SOURCES ${SOURCES} ${GUI_SOURCES})
else()
    set(TIMERS_SOURCES ${SOURCES})
endif ()

add_library(${PROJECT_NAME} MODULE ${TIMERS_SOURCES})
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
  target_link_libraries(${PROJECT_NAME} Spaghetti SpaghettiUI)
else()
  target_link_libraries(${PROJECT_NAME} Spaghetti)
endif ()

install(TARGETS ${PROJECT_NAME}
  COMPONENT ${PROJECT_NAME}Plugin
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/spaghetti
  )

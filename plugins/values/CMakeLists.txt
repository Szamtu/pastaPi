cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(Values VERSION ${PastaPi_VERSION} LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Core REQUIRED)

set(SOURCES
    bool/const_bool/const_bool.cc
    bool/const_bool/const_bool.h
    bool/random_bool/random_bool.cc
    bool/random_bool/random_bool.h

    double/const_double/const_double.cc
    double/const_double/const_double.h
    double/random_double/random_double.cc
    double/random_double/random_double.h
    double/min_double/min_double.cc
    double/min_double/min_double.h
    double/max_double/max_double.cc
    double/max_double/max_double.h

    integer/const_int/const_int.cc
    integer/const_int/const_int.h
    integer/max_int/max_int.cc
    integer/max_int/max_int.h
    integer/min_int/min_int.cc
    integer/min_int/min_int.h
    integer/random_int/random_int.cc
    integer/random_int/random_int.h

    string/const_string/const_string.cc
    string/const_string/const_string.h
    string/concat_string/concat_string.cc
    string/concat_string/concat_string.h

    convert/convert_value/convert_value.cc
    convert/convert_value/convert_value.h
    convert/degree_to_radian/degree_to_radian.cc
    convert/degree_to_radian/degree_to_radian.h
    convert/radian_to_degree/radian_to_degree.cc
    convert/radian_to_degree/radian_to_degree.h

    values_ui/info_double/info_double.cc
    values_ui/info_double/info_double.h
    values_ui/info_int/info_int.cc
    values_ui/info_int/info_int.h
    values_ui/info_string/info_string.cc
    values_ui/info_string/info_string.h
    values_ui/push_button/push_button.cc
    values_ui/push_button/push_button.h
    values_ui/seven_segment_display/seven_segment_display.cc
    values_ui/seven_segment_display/seven_segment_display.h
    values_ui/toggle_button/toggle_button.cc
    values_ui/toggle_button/toggle_button.h

    values.cc
)

set(GUI_SOURCES
    bool/const_bool/const_bool_node.cc
    bool/const_bool/const_bool_node.h
    double/const_double/const_double_node.cc
    double/const_double/const_double_node.h
    double/random_double/random_double_node.cc
    double/random_double/random_double_node.h
    integer/const_int/const_int_node.cc
    integer/const_int/const_int_node.h
    integer/random_int/random_int_node.cc
    integer/random_int/random_int_node.h
    string/const_string/const_string_node.cc
    string/const_string/const_string_node.h
    values_ui/info_double/info_double_node.cc
    values_ui/info_double/info_double_node.h
    values_ui/info_int/info_int_node.cc
    values_ui/info_int/info_int_node.h
    values_ui/info_string/info_string_node.cc
    values_ui/info_string/info_string_node.h
    values_ui/push_button/push_button_node.cc
    values_ui/push_button/push_button_node.h
    values_ui/seven_segment_display/seven_segment_display_node.cc
    values_ui/seven_segment_display/seven_segment_display_node.h
    values_ui/toggle_button/toggle_button_node.cc
    values_ui/toggle_button/toggle_button_node.h
    )

if (BUILD_PLUGINS_GUI)
    set(VALUES_SOURCES ${SOURCES} ${GUI_SOURCES})
else()
    set(VALUES_SOURCES ${SOURCES})
endif ()

add_library(${PROJECT_NAME} MODULE ${VALUES_SOURCES})
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
  target_link_libraries(${PROJECT_NAME} Spaghetti SpaghettiUI Qt5::Core)
else()
  target_link_libraries(${PROJECT_NAME} Spaghetti Qt5::Core)
endif ()

install(TARGETS ${PROJECT_NAME}
  COMPONENT ${PROJECT_NAME}Plugin
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/spaghetti
  )

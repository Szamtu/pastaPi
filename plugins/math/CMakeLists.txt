cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(MathPlugin VERSION ${PastaPi_VERSION} LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Core REQUIRED)

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

    rounding/ceil/ceil.cc
    rounding/ceil/ceil.h

    math.cc
)

include(PluginBuild)

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(LogicGates VERSION 17.09.06 LANGUAGES C CXX)

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

include(PluginBuild)

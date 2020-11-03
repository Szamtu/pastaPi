cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(libSpaghetti
    VERSION ${PastaPi_VERSION}
    LANGUAGES C CXX
)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

configure_file(include/spaghetti/version.h.in include/spaghetti/version.h)

set(LIBSPAGHETTI_PUBLIC_COMMON_HEADERS
  include/spaghetti/api.h
  include/spaghetti/sockvalues.h
  include/spaghetti/element.h
  include/spaghetti/logger.h
  include/spaghetti/package.h
  include/spaghetti/registry.h
  include/spaghetti/strings.h
  include/spaghetti/utils.h
  include/spaghetti/dummynode.h
  )

set(LIBSPAGHETTI_GENERATED_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/version.h
  )

set(LIBSPAGHETTI_SOURCES
  ${LIBSPAGHETTI_PUBLIC_COMMON_HEADERS}
  include/spaghetti/version.h.in

  source/sockvalues.cc
  source/element.cc
  source/logger.cc
  source/package.cc
  source/registry.cc
  source/shared_library.cc
  source/shared_library.h
  )

set(LIBSPAGHETTI_ALL_SOURCES
  ${LIBSPAGHETTI_GENERATED_SOURCES}
  ${LIBSPAGHETTI_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIBSPAGHETTI_SOURCES})
source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${LIBSPAGHETTI_GENERATED_SOURCES})

add_library(Spaghetti SHARED ${LIBSPAGHETTI_ALL_SOURCES})
target_compile_features(Spaghetti PUBLIC cxx_std_17)
target_compile_definitions(Spaghetti
  PUBLIC SPAGHETTI_SHARED
  PRIVATE SPAGHETTI_EXPORTS ${SPAGHETTI_DEFINITIONS}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_DEFINITIONS_DEBUG}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_DEFINITIONS_RELEASE}>
  )
if(PASTAPI_BUILD_WITH_TBB)
  add_compile_definitions(BUILD_WITH_TBB)
endif ()
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

if(PASTAPI_BUILD_WITH_TBB)
    target_link_options( Spaghetti PUBLIC -ltbb )
endif ()

target_link_libraries(Spaghetti
  PUBLIC ${CMAKE_THREAD_LIBS_INIT} opencv_core
)

install(TARGETS Spaghetti
  COMPONENT SDK
  EXPORT SpaghettiConfig
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

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

# define minimum cmake version
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(kiptool VERSION 2.9 LANGUAGES CXX)

set(CMAKE_FIND_DEBUG_MODE OFF)

# require C++20
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif(NOT DEFINED CMAKE_CXX_STANDARD)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE
        STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE
  )
endif(NOT CMAKE_BUILD_TYPE)

file(TO_CMAKE_PATH "$ENV{QTPATH}/lib/cmake/;${CMAKE_BINARY_DIR}/generators" PREFIX_PATH)
file(TO_CMAKE_PATH "${PROJECT_SOURCE_DIR}/cmake/;${CMAKE_BINARY_DIR}/generators" MODULE_PATH)

# Set environment variables so that CMAKE can find the packages
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PREFIX_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_PATH})
message("CMAKE_PREFIX_PATH is: ${CMAKE_PREFIX_PATH}")
message("CMAKE_MODULE_PATH is: ${CMAKE_MODULE_PATH}")
message("QTPATH is: $ENV{QTPATH}")
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

find_package(Python3 COMPONENTS Interpreter Development)

find_package(Armadillo REQUIRED)
find_package(pybind11 REQUIRED)
find_package(libxml2 REQUIRED)
find_package(OpenBLAS REQUIRED)
find_package(TIFF REQUIRED)
find_package(FFTW3 REQUIRED)
find_package(cfitsio REQUIRED)
find_package(ZLIB REQUIRED)
find_package(HDF5 REQUIRED)


# for testing we will need the python interpreter
find_package(PythonInterp REQUIRED)

if (WIN32)
  find_package(dirent)
endif()

if (WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Release)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Release)
elseif (LINUX)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
elseif (APPLE)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  include_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()

if("${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" VERSION_LESS "3.5")
  message(
    FATAL_ERROR
      "Python v3 interpreter must be greater than or equal to 3.5. Found ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}"
  )
endif()

# Get version from git-describe
execute_process(
  COMMAND git describe --tag
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  RESULT_VARIABLE HAVE_GIT_VERSION_INFO
  OUTPUT_VARIABLE GIT_VERSION_INFO
  OUTPUT_STRIP_TRAILING_WHITESPACE
  COMMAND_ERROR_IS_FATAL ANY
)
if(${HAVE_GIT_VERSION_INFO} EQUAL 0)
  message(STATUS "Got version from Git: ${GIT_VERSION_INFO}")
  add_definitions(-DVERSION="${GIT_VERSION_INFO}")
endif()

include(CTest)

execute_process(
  COMMAND git describe --tag
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  RESULT_VARIABLE HAVE_GIT_VERSION_INFO
  OUTPUT_VARIABLE GIT_VERSION_INFO OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(${HAVE_GIT_VERSION_INFO} EQUAL 0)
  message(STATUS "Got version from Git: ${GIT_VERSION_INFO}")
  add_definitions(-DIMGSUITE_VERSION="${GIT_VERSION_INFO}")
endif()


if(WIN32)
add_compile_options(
  /openmp 
  /O2
  )
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
  add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wunused
    -Wshadow
    -Winit-self
    -Wpointer-arith
    -Woverloaded-virtual
    -Wold-style-cast
    -Wcast-qual
    -Wcast-align
  )
  add_compile_options(
    $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-operator-names>
  )
endif()

set(PYTHONDIR kipl)
set(INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})

set(EXPORT_NAME ${PROJECT_NAME}-targets)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN/../lib)
else()
  set(CMAKE_INSTALL_RPATH @loader_path/../Frameworks @loader_path/../../../lib @loader_path/lib @loader_path)
endif()
# turn on testing
enable_testing()

## Core libraries
add_subdirectory(frameworks/imageprocessing/ProcessFramework)

## modules
add_subdirectory(frameworks/imageprocessing/modules/AdvancedFilterModules)
add_subdirectory(frameworks/imageprocessing/modules/AdvancedFilterModulesGUI)
add_subdirectory(frameworks/imageprocessing/modules/BaseModules)
add_subdirectory(frameworks/imageprocessing/modules/BaseModulesGUI)
add_subdirectory(frameworks/imageprocessing/modules/ClassificationModules)
add_subdirectory(frameworks/imageprocessing/modules/ClassificationModulesGUI)
add_subdirectory(frameworks/imageprocessing/modules/ImagingModules)
add_subdirectory(frameworks/imageprocessing/modules/ImagingModulesGui)
# add_subdirectory(frameworks/imageprocessing/modules/PCAModules)
# add_subdirectory(frameworks/imageprocessing/modules/StatisticsModules)
# add_subdirectory(frameworks/imageprocessing/modules/PorespaceModules)


## applications
add_subdirectory(applications/kiptool)
#if (WIN32)
#    add_subdirectory(applications/kiptoolCLI)
#endif()
#add_subdirectory(applications/kiptool/UnitTests)

## Binder modules
#add_subdirectory(frameworks/tomography/pyMuhRec)
#add_subdirectory(core/algorithms/pyimgalg)
#add_subdirectory(core/algorithms/pyadvancedfilters)

# define minimum cmake version
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

#if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
#message(FATAL_ERROR “In-source_build_detected!_Create_a_build_folder_and_call_CMake_there”)
#endif()

execute_process(
  COMMAND git describe --tags --abbrev=0 | cut -d'v' -f 2
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE KIPTOOL_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

project(kiptool VERSION 2.9 LANGUAGES CXX)

# require C++17
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

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

set(CMAKE_MODULE_PATH "lib" )
# for testing we will need the python interpreter
find_package(PythonInterp REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/applications)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if("${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" VERSION_LESS "3.5")
  message(
    FATAL_ERROR
      "Python v3 interpreter must be greater than or equal to 3.5. Found ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}"
  )
endif()

if (WIN32)
  set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../build-kiptool) # needs to be fixed 

  find_package(PythonInterp REQUIRED)
  find_package(LibXml2 REQUIRED)
  find_package(armadillo REQUIRED)
  find_package(ZLIB REQUIRED)
  find_package(TIFF REQUIRED)
  find_package(FFTW3 REQUIRED)
  find_package(OpenBLAS REQUIRED)
  find_package(pybind11 REQUIRED)
  find_package(cfitsio REQUIRED)
  # find_package(Nexus REQUIRED)
else()
  set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
  
  # for testing we will need the python interpreter
  find_package(PythonInterp REQUIRED)
  find_package(LibXml2 REQUIRED)
  find_package(Armadillo REQUIRED)
  find_package(ZLIB REQUIRED)
  find_package(TIFF REQUIRED)
  find_package(FFTW REQUIRED)
  find_package(LAPACK REQUIRED)
  find_package(BLAS REQUIRED)
  find_package(pybind11 REQUIRED)
  find_package(CFITSIO REQUIRED)
  find_package(Nexus REQUIRED)
endif()

if(NOT WIN32)
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
endif(NOT WIN32)

set(PYTHONDIR kipl)
set(INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})

set(EXPORT_NAME ${PROJECT_NAME}-targets)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN/../lib)
else()
  set(CMAKE_INSTALL_RPATH @loader_path/../Frameworks)
endif()

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
#add_subdirectory(applications/kiptoolCLI)
#add_subdirectory(applications/kiptool/UnitTests)

## Binder modules
#add_subdirectory(frameworks/tomography/pyMuhRec)
#add_subdirectory(core/algorithms/pyimgalg)
#add_subdirectory(core/algorithms/pyadvancedfilters)

if(DYNAMIC_LIB)
  # install(
  #   EXPORT ${EXPORT_NAME}
  #   DESTINATION "lib/cmake/kipl"
  #   NAMESPACE kipl::
  # )

  # include(CMakePackageConfigHelpers)
  # configure_package_config_file(
  #   ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
  #   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  #   INSTALL_DESTINATION lib/cmake/kipl
  # )
  # write_basic_package_version_file(
  #   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  #   VERSION "${CMAKE_PROJECT_VERSION}"
  #   COMPATIBILITY SameMinorVersion # switch to SameMajorVersion after 1.0
  #                                  # release
  # )
  # install(
  #   FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  #         "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  #   DESTINATION lib/cmake/kipl
  # )
  if (LINUX)
    print('Linux')
    install(FILES 
      ../lib/libkipl.so.1.0.0 
      ../lib/libModuleConfig.so.1.0.0 
      ../lib/libReconFramework.so.1.0.0
      ../lib/libStdBackProjectors.so.1.0.0
      ../lib/libFDKBackProjectors.so.1.0.0
      ../lib/libImagingAlgorithms.so.1.0.0
      ../lib/libReaderConfig.so.1.0.0
      ../lib/libNeXus.so.1.0.0	
      ../lib/libhdf5_cpp.so.11
      ../lib/libNeXusCPP.so.1.0.0
      ../lib/libhdf5_hl.so.10
      ../lib/libhdf5.so.10	
      ../lib/libsz.so.2

  #    /opt/local/lib/libxml2.2.dylib 
  #    /opt/local/lib/libtiff.5.dylib
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

    execute_process(
      COMMAND for f in `ls *.so.1.0.0`; do ln -s $f `basename $f .so.1.0.0`.so.1; done 
      WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib"
    )
  endif()
  if (APPLE)
    install(FILES 
#    ../lib/libkipl.1.0.0.dylib
#    ../lib/libModuleConfig.1.0.0.dylib
#    ../lib/libReconFramework.1.0.0.dylib
#    ../lib/libStdBackProjectors.1.0.0.dylib
#    ../lib/libFDKBackProjectors.1.0.0.dylib
#    ../lib/libImagingAlgorithms.1.0.0.dylib
#    ../lib/libReaderConfig.1.0.0.dylib
    # ../imagingsuite/external/mac/lib/libNeXus.1.0.0.dylib	
    # ../imagingsuite/external/mac/lib//libhdf5_cpp.11.dylib
    # ../imagingsuite/external/mac/lib//libNeXusCPP.1.0.0.dylib
    # ../imagingsuite/external/mac/lib//libhdf5_hl.10.dylib
    # ../imagingsuite/external/mac/lib//libhdf5.10.dylib	
    # ../imagingsuite/external/mac/lib//libsz.2.dylib
    # /opt/local/lib/libxml2.2.dylib 
    # /opt/local/lib/libtiff.5.dylib
    # /opt/local/lib/libfftw3.3.dylib
    # /opt/local/lib/libfftw3f.3.dylib
    # /opt/local/lib/libcfitsio.dylib
    # /opt/local/lib/libarmadillo.10.dylib

    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

    execute_process(
      COMMAND for f in `ls *.1.0.0.*`; do ln -s $f `basename $f .1.0.0.dylib`.1.dylib; done 
      WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib"
    )
  endif()
  


endif(DYNAMIC_LIB)

# turn on testing
#enable_testing()

message(${CMAKE_CURRENT_SOURCE_DIR})
## define test
#add_test(
#  NAME
#    testBindAverageImage
#  COMMAND
#    ${CMAKE_COMMAND} -E env IMAGALG_MODULE_PATH=$<TARGET_FILE_DIR:imagalg>
#    ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/UnitTests/python/testBindAverageImage.py
#    ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/UnitTests/python/testBindAverageImage.py
#)
#
#add_test(
#  NAME
#    testBindPolynomialCorrection
#  COMMAND
#    ${CMAKE_COMMAND} -E env IMAGALG_MODULE_PATH=$<TARGET_FILE_DIR:imagalg>
#    ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/UnitTests/python/testBindPolynomialCorrection.py
#)

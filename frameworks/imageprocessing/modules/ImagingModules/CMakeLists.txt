cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "ImagingModules")
project(kiptool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
include_directories(
    ../../ProcessFramework/include/
    ../../../../../imagingsuite/core/kipl/kipl/include
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../../imagingsuite/core/modules/ReaderConfig
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../../imagingsuite/core/algorithms/AdvancedFilters/
    ../../../../../ExternalDependencies/macos/include
    )
endif()

if (WIN32)
include_directories(
    ../../ProcessFramework/include/
    ../../../../../imagingsuite/core/kipl/kipl/include
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../../imagingsuite/core/modules/ReaderConfig
    ../../../../../imagingsuite/external/include/
    )
endif()

link_directories(../../../../../install/lib  )

set(INC_FILES
    src/ImagingModules_global.h
    src/ringcleanmodule.h
    src/bblognorm.h
    src/stdafx.h
    src/imagingmodules.h
    src/stripefiltermodule.h
    src/morphspotcleanmodule.h
    src/targetver.h
    src/referenceimagecorrectionmodule.h
    src/translateprojectionmodule.h
)

set(SRC_FILES 
    src/bblognorm.cpp
    src/ringcleanmodule.cpp
    src/imagingmodules.cpp
    src/stdafx.cpp
    src/morphspotcleanmodule.cpp
    src/stripefiltermodule.cpp
    src/referenceimagecorrectionmodule.cpp
    src/translateprojectionmodule.cpp
)

add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
  )

target_compile_definitions(${TARGET_NAME} PRIVATE IMAGINGMODULES_LIBRARY NOMINMAX HAVE_NEXUS)

  message(${CMAKE_HOST_SYSTEM_PROCESSOR})

  if (WIN32)
  target_link_directories(${TARGET_NAME} PRIVATE
  ../../../../../imagingsuite/external/lib64
  )
  
  target_link_libraries(${TARGET_NAME}
  PUBLIC
      kipl
      ImagingAlgorithms
      ModuleConfig
      ProcessFramework
      ReaderConfig

      LibXml2::LibXml2
      TIFF::TIFF

      libfftw3-3
      libfftw3f-3
      )
endif()

if (APPLE)
  
      target_link_libraries(${TARGET_NAME}
          PUBLIC
          kipl
          ImagingAlgorithms
          ModuleConfig
          ReaderConfig
          ProcessFramework
        #   xml2
        #   fftw3
        #   fftw3f
        #   cfitsio
        #   tiff
        #   armadillo
        #   lapack
        #   blas
        #   NeXus.1.0.0
        #   NeXusCPP.1.0.0
      )
  endif()

  install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

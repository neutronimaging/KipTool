cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "ImagingModules")
project(kiptool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
include_directories(
    /opt/local/include
    /opt/local/include/libxml2
    /usr/include/libxml2
    ../../ProcessFramework/include/
#    ../../../../external/src/linalg
    ../../../../../imagingsuite/core/kipl/kipl/include
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../../imagingsuite/core/modules/ReaderConfig
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../../imagingsuite/core/algorithms/AdvancedFilters/
    ../../../../../ExternalDependencies/macos/include
    ../../../../../ExternalDependencies/macos/include/nexus
    ../../../../../imagingsuite/external/mac/include/
    ../../../../../imagingsuite/external/mac/include/hdf5
    )
endif()

if (WIN32)
include_directories(
    ../../ProcessFramework/include/
    ../../../../../imagingsuite/core/kipl/kipl/include
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    # ../../../../../imagingsuite/core/algorithms/AdvancedFilters/
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../../imagingsuite/core/modules/ReaderConfig
    ../../../../../ExternalDependencies/windows/include
    ../../../../../ExternalDependencies/windows/include/nexus
    ../../../../../ExternalDependencies/windows/include/cfitsio
    ../../../../../ExternalDependencies/windows/include/libxml2
    ../../../../../imagingsuite/external/include/
    )
endif()

link_directories(../../../../../install/lib /opt/local/lib )

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
  ../../../../../build-imagingsuite/lib/Release
  ../../../../../install/lib
  ../../../../../ExternalDependencies/windows/lib
  ../../../../../imagingsuite/external/lib64
  ../../../../../imagingsuite/external/lib64/nexus
  )
  
  target_link_libraries(${TARGET_NAME}
  PRIVATE
      kipl
      ImagingAlgorithms
      ModuleConfig
      ProcessFramework
      ReaderConfig
      libxml2
      # fftw3
      # fftw3f
      # cfitsio
      libtiff
      # armadillo
      # lapack
      # blas
      # NeXus.1.0.0
      # NeXusCPP.1.0.0
      # liblapack
      # libopenblas
      # libfftw3-3
      # libfftw3f-3
      # NeXus
      # NeXusCPP
      # Iphlpapi

      )
endif()

  if (APPLE)
      if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
          target_link_directories(${TARGET_NAME} PRIVATE
              /opt/local/lib
              ../../../../../ExternalDependencies/macos/arm64/lib
              )
      else()
          target_link_directories(${TARGET_NAME} PRIVATE
              /opt/local/lib
              ../../../../../ExternalDependencies/macos/x86_64/lib
              )
      endif()
  
      target_link_libraries(${TARGET_NAME}
          PRIVATE
          kipl
          ImagingAlgorithms
          ModuleConfig
          ReaderConfig
          ProcessFramework
          xml2
          fftw3
          fftw3f
          cfitsio
          tiff
          armadillo
          lapack
          blas
          NeXus.1.0.0
          NeXusCPP.1.0.0
      )
  endif()

  install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

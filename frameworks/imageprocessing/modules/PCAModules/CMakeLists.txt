cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "PCAModules")
project(kiptool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    /opt/local/include
    /opt/local/include/libxml2
    ../../ProcessFramework/include/
#    ../../../../external/src/linalg
    ../../../../../imagingsuite/core/kipl/kipl/include
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../../imagingsuite/core/algorithms/AdvancedFilters/
    ../../../../../ExternalDependencies/macos/include
    ../../../../../ExternalDependencies/macos/include/nexus
    ../../../../../imagingsuite/external/mac/include/
    ../../../../../imagingsuite/external/mac/include/hdf5
    )

link_directories(../../../../../install/lib /opt/local/lib )

set(INC_FILES
    src/PCAFilterModule.h
    src/pcamodules.h
    src/targetver.h
    src/PCAModules_global.h
    src/stdafx.h
)

set(SRC_FILES 
    src/PCAFilterModule.cpp
    src/pcamodules.cpp
    src/stdafx.cpp
)

# target_compile_definitions(${TARGET_NAME} PRIVATE ADVANCEDFILTERMODULES_LIBRARY HAVE_NEXUS)

add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
  )

  message(${CMAKE_HOST_SYSTEM_PROCESSOR})
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

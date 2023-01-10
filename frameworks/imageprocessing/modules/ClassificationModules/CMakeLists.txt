cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "ClassificationModules")
project(kiptool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
include_directories(
    ../../ProcessFramework/include/
    ../../../../../imagingsuite/core/kipl/kipl/include
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../../imagingsuite/core/algorithms/AdvancedFilters/
    ../../../../../ExternalDependencies/macos/include
    )
endif()

if (WIN32)
include_directories(
    ../../ProcessFramework/include/
    ../../../../../imagingsuite/core/kipl/kipl/include
    # ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    # ../../../../../imagingsuite/core/algorithms/AdvancedFilters/
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../../ExternalDependencies/windows/include
    ../../../../../ExternalDependencies/windows/include/nexus
    ../../../../../ExternalDependencies/windows/include/cfitsio
    ../../../../../ExternalDependencies/windows/include/libxml2
    ../../../../../imagingsuite/external/include/
    )
endif()

link_directories(../../../../../install/lib )

set(INC_FILES
    src/BasicThreshold.h
 #   src/KernelFuzzyCMeans.h
    src/ClassificationModules.h
    src/RemoveBackground.h
    src/ClassificationModules_global.h
    src/stdafx.h
    src/DoubleThreshold.h
    src/targetver.h
 #   src/FuzzyCMeans.h
)

set(SRC_FILES 
    src/BasicThreshold.cpp
 #   src/KernelFuzzyCMeans.cpp
    src/ClassificationModules.cpp
    src/RemoveBackground.cpp
    src/DoubleThreshold.cpp
    src/dllmain.cpp
  #  src/FuzzyCMeans.cpp
    src/stdafx.cpp
)

add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
  )

target_compile_definitions(${TARGET_NAME} PRIVATE CLASSIFICATIONMODULES_LIBRARY NOMINMAX HAVE_NEXUS)

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

      target_link_libraries(${TARGET_NAME}
          PUBLIC
          kipl
          ImagingAlgorithms
          ModuleConfig
          ProcessFramework
      )
  endif()

  install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

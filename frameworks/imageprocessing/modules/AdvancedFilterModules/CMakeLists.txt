cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "AdvancedFilterModules")
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
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../../imagingsuite/core/algorithms/AdvancedFilters/
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
    src/AdvancedFilterModules.h
    src/NonLinDiffusion.h
    src/targetver.h
    src/AdvancedFilterModules_global.h
  #  src/nonlocalmeansmodule.h
    src/ISSfilterModule.h
    src/stdafx.h
)

set(SRC_FILES 
    src/AdvancedFilterModules.cpp
    src/NonLinDiffusion.cpp
   # src/nonlocalmeansmodule.cpp
    src/ISSfilterModule.cpp
    src/dllmain.cpp
    src/stdafx.cpp
)

add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
  )

set_target_properties(${TARGET_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_compile_definitions(${TARGET_NAME} PRIVATE ADVANCEDFILTERMODULES_LIBRARY NOMINMAX HAVE_NEXUS)

message("AdvancedFilterModules ${CMAKE_HOST_SYSTEM_PROCESSOR}")

if (WIN32)
    target_link_directories(${TARGET_NAME} PRIVATE
    ../../../../../build-imagingsuite/lib/Release
    ../../../../../install/lib
    ../../../../../ExternalDependencies/windows/lib
    ../../../../../imagingsuite/external/lib64
    ../../../../../imagingsuite/external/lib64/nexus
    )
    
    target_link_libraries(${TARGET_NAME}
    PUBLIC
        kipl
        ModuleConfig
        ImagingAlgorithms
        AdvancedFilters
        ProcessFramework
        # LibXml2::LibXml2
        TIFF::TIFF
        # ${LAPACK_LIBRARIES}
        # # LAPACK::LAPACK
        # ${BLAS_LIBRARIES}
        # # BLAS::BLAS
        # ${ARMADILLO_LIBRARIES}
        FFTW::Double
        FFTW::Float
        # libxml2
        # cfitsio
        # libtiff
        # liblapack
        # libopenblas
        # libfftw3-3
        # libfftw3f-3
        # NeXus
        # NeXusCPP
        # Iphlpapi

        )
endif()

if (UNIX)
      target_link_libraries(${TARGET_NAME}
          PUBLIC
            kipl
            ImagingAlgorithms
            AdvancedFilters
            ModuleConfig
            ProcessFramework
            # LibXml2::LibXml2
            TIFF::TIFF
            # ${LAPACK_LIBRARIES}
            # # LAPACK::LAPACK
            # ${BLAS_LIBRARIES}
            # # BLAS::BLAS
            # ${ARMADILLO_LIBRARIES}
            FFTW::Double
            FFTW::Float
      )
  endif()



  install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

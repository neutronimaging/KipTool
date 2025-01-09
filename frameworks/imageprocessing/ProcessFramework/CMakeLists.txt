cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "ProcessFramework")
project(kipltool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
include_directories(
    ../../../../imagingsuite/core/kipl/kipl/include
    ../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../imagingsuite/core/modules/ModuleConfig/include
    # ../../../../ExternalDependencies/macos/include
    )
endif()

if (WIN32)
include_directories(
    ../../../../imagingsuite/core/kipl/kipl/include
    ../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../imagingsuite/core/modules/ModuleConfig/include
    # ../../../../imagingsuite/external/include/
    )
endif()

link_directories(../../../../install/lib)

set(INC_FILES
    include/KiplEngine.h
    include/KiplFrameworkException.h
    include/KiplProcessConfig.h
    include/ProcessFramework_global.h
    include/KiplFactory.h
    include/KiplModuleItem.h
    include/KiplProcessModuleBase.h
)

set(SRC_FILES
    src/KiplEngine.cpp
    src/KiplModuleItem.cpp
    src/dllmain.cpp
    src/targetver.h
    src/KiplFactory.cpp
    src/KiplProcessConfig.cpp
    src/stdafx.cpp
    src/KiplFrameworkException.cpp
    src/KiplProcessModuleBase.cpp
    src/stdafx.h
)


add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
)


set_target_properties(${TARGET_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_compile_definitions(${TARGET_NAME} PRIVATE PROCESSFRAMEWORK_LIBRARY)

message(${CMAKE_HOST_SYSTEM_PROCESSOR})

if (WIN32)
    
    target_link_libraries(${TARGET_NAME}
    PRIVATE
        kipl
        ModuleConfig
        ImagingAlgorithms
        LibXml2::LibXml2
        TIFF::TIFF
        cfitsio::cfitsio
        )
endif()

if (APPLE)
    target_link_libraries(${TARGET_NAME}
        PUBLIC
            kipl
            ImagingAlgorithms
            ModuleConfig
            ReaderConfig
            LibXml2::LibXml2
            TIFF::TIFF
            cfitsio::cfitsio
    )
endif()

if (LINUX)
    target_link_libraries(${TARGET_NAME}
        PRIVATE
        kipl
        ImagingAlgorithms
        ModuleConfig
        # xml2
        # fftw3
        # fftw3f
        # cfitsio
        # tiff
        # armadillo
        # lapack
        # blas
        # NeXus
        # NeXusCPP
    )
endif()
install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

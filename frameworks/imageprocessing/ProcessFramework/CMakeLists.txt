cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "ProcessFramework")
project(kipl LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    /opt/local/include
    /opt/local/include/libxml2
    /usr/include/libxml2
    ../../../../imagingsuite/core/kipl/kipl/include
    ../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    #../../../../external/src/linalg
    ../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../ExternalDependencies/macos/include
    ../../../../ExternalDependencies/macos/include/nexus
    ../../../../imagingsuite/external/mac/include/
    ../../../../imagingsuite/external/mac/include/nexus
    ../../../../imagingsuite/external/mac/include/hdf5
    )

#link_directories(/opt/local/lib ../../../../external/mac/lib)

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
set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_RPATH "\$ORIGIN")

target_compile_definitions(${TARGET_NAME} PRIVATE PROCESSFRAMEWORK_LIBRARY HAVE_NEXUS)

add_definitions(-DHAVE_NEXUS)

message(${CMAKE_HOST_SYSTEM_PROCESSOR})
if (APPLE)
    if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
        target_link_directories(${TARGET_NAME} PRIVATE
            /opt/local/lib
            ../../../../install/lib
            ../../../../ExternalDependencies/macos/arm64/lib
            )
    else()
        target_link_directories(${TARGET_NAME} PRIVATE
            /opt/local/lib
            ../../../../external/mac/lib/
            )
    endif()
    target_link_libraries(${TARGET_NAME}
        PRIVATE
        kipl
        ImagingAlgorithms
        ModuleConfig
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

if (LINUX)
    target_link_libraries(${TARGET_NAME}
        PRIVATE
        kipl
        ImagingAlgorithms
        ModuleConfig
        xml2
        fftw3
        fftw3f
        cfitsio
        tiff
        armadillo
        lapack
        blas
        NeXus
        NeXusCPP
    )
endif()
install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

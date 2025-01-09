cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "BaseModules")
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
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    )
endif()

link_directories(../../../../../install/lib)

set(INC_FILES
    src/BaseModules.h
    src/ScaleData.h
    src/BaseModules_global.h
    src/VolumeProject.h
    src/BuildScene.h
    src/stdafx.h
    src/ClampData.h
    src/targetver.h
    src/DoseCorrection.h
)

set(SRC_FILES 
    src/BaseModules.cpp	
    src/DoseCorrection.cpp	
    src/dllmain.cpp
    src/BuildScene.cpp	
    src/ScaleData.cpp	
    src/stdafx.cpp
    src/ClampData.cpp	
    src/VolumeProject.cpp
)

add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
  )

target_compile_definitions(${TARGET_NAME} PRIVATE BASEMODULES_LIBRARY NOMINMAX)



  message(${CMAKE_HOST_SYSTEM_PROCESSOR})

  if (WIN32)
    target_link_directories(${TARGET_NAME} PRIVATE
    ../../../../../install/lib
    )
    
    target_link_libraries(${TARGET_NAME}
    PUBLIC
        kipl
        ImagingAlgorithms
        ModuleConfig
        ProcessFramework
        LibXml2::LibXml2
        TIFF::TIFF
        )
endif()

if (UNIX)
    target_link_libraries(${TARGET_NAME}
        PUBLIC
        kipl
        ImagingAlgorithms
        ModuleConfig
        ProcessFramework
    )
endif()

install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

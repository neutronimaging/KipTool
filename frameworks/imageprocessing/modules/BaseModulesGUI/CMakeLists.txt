cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "BaseModulesGUI")
project(kiptool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets Charts REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts )

message("Using Qt${QT_VERSION_MAJOR}")
if (APPLE)
include_directories(
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../../imagingsuite/core/algorithms/AdvancedFilters
    ../../../../../imagingsuite/GUI/qt/QtAddons/
    ../../../../../imagingsuite/GUI/qt/QtModuleConfigure/
    ../../../../../imagingsuite/core/kipl/kipl/include
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../../ExternalDependencies/macos/include
    ../AdvancedFilterModules/src
    ../../ProcessFramework/include/
    )
endif()

if (WIN32)
include_directories(
    ../../ProcessFramework/include/
    ../BaseModules/src
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../../imagingsuite/core/algorithms/AdvancedFilters
    ../../../../../imagingsuite/GUI/qt/QtAddons/
    ../../../../../imagingsuite/GUI/qt/QtModuleConfigure/
    ../../../../../imagingsuite/core/kipl/kipl/include
    ../../../../../imagingsuite/core/modules/ModuleConfig/include


    # ../../../../../ExternalDependencies/windows/include
    # ../../../../../ExternalDependencies/windows/include/nexus
    # ../../../../../ExternalDependencies/windows/include
    # ../../../../../ExternalDependencies/windows/include/nexus
    # ../../../../../ExternalDependencies/windows/include/cfitsio
    # ../../../../../ExternalDependencies/windows/include/libxml2
    # ../../../../../imagingsuite/external/include/
    )
endif()

link_directories( ../../../../../install/lib )

set(INC_FILES
    src/basemodulesgui.h
    src/basemodulesgui_global.h
    src/clampdatadlg.h
    src/dosecorrectiondlg.h
    src/scaledatadlg.h
)

set(SRC_FILES
    src/basemodulesgui.cpp
    src/clampdatadlg.cpp
    src/dosecorrectiondlg.cpp
    src/scaledatadlg.cpp
)

set(UI_FILES
    src/clampdatadlg.ui
    src/dosecorrectiondlg.ui
    src/scaledatadlg.ui
)

add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
)

set_target_properties(${TARGET_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_compile_definitions(${TARGET_NAME} PRIVATE BASEMODULESGUI_LIBRARY NOMINMAX)

message(${CMAKE_HOST_SYSTEM_PROCESSOR})

if (WIN32)
target_link_directories(${TARGET_NAME} PRIVATE
    ../../../../../build-imagingsuite/lib/Release
    ../../../../../ExternalDependencies/windows/lib
    ../../../../../imagingsuite/external/lib64
)

target_link_libraries(${TARGET_NAME} 
PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Charts

  kipl
  BaseModules
  ProcessFramework
  ModuleConfig
  QtAddons
  QtModuleConfigure
)
endif()

if (UNIX)
    target_link_libraries(${TARGET_NAME}
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Charts
    #    Qt${QT_VERSION_MAJOR}::PrintSupport
        kipl
        ImagingAlgorithms
        AdvancedFilterModules
        AdvancedFilters
        ModuleConfig
        ProcessFramework
        QtAddons
        QtModuleConfigure
    )
endif()

install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "AdvancedFilterModulesGUI")
project(kiptool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
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
    ../AdvancedFilterModules/src
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../../imagingsuite/core/algorithms/AdvancedFilters
    ../../../../../imagingsuite/GUI/qt/QtAddons/
    ../../../../../imagingsuite/GUI/qt/QtModuleConfigure/
    ../../../../../imagingsuite/core/kipl/kipl/include
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    )
endif()

link_directories( ../../../../../install/lib )

set(INC_FILES
    src/advancedfiltermodulesgui.h
    src/issfilterdlg.h
    src/nonlindiffusiondlg.h
    src/advancedfiltermodulesgui_global.h
)

set(SRC_FILES
    src/advancedfiltermodulesgui.cpp
    src/issfilterdlg.cpp
    src/nonlindiffusiondlg.cpp
)

set(UI_FILES
    src/issfilterdlg.ui
    src/nonlindiffusiondlg.ui
)

add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
  ${UI_FILES}
)


set_target_properties(${TARGET_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_compile_definitions(${TARGET_NAME} PRIVATE ADVANCEDFILTERMODULESGUI_LIBRARY NOMINMAX HAVE_NEXUS)

add_definitions(-DHAVE_NEXUS)

message(${CMAKE_HOST_SYSTEM_PROCESSOR})

if (WIN32)
target_link_directories(${TARGET_NAME} PRIVATE
    ../../../../../install/lib
    ../../../../../ExternalDependencies/windows/lib
    ../../../../../imagingsuite/external/lib64
)

target_link_libraries(${TARGET_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Charts

  kipl
  ImagingAlgorithms
  AdvancedFilterModules
  AdvancedFilters
  ProcessFramework
  ModuleConfig
  QtAddons
  QtModuleConfigure
  LibXml2::LibXml2
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
        ProcessFramework
        ModuleConfig
        QtAddons
        QtModuleConfigure
    )
endif()

install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

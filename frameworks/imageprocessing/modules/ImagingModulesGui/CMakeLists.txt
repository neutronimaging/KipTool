cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "ImagingModulesGUI")
project(kiptool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 COMPONENTS Widgets Charts PrintSupport REQUIRED)
# find_package(Qt6 REQUIRED COMPONENTS Widgets Charts PrintSupport)

find_package(QT NAMES Qt6 COMPONENTS Widgets Charts REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts )

message("Using Qt${QT_VERSION_MAJOR}")

include_directories(
    /opt/local/include
    /opt/local/include/libxml2
    /usr/include/libxml2
    ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../../imagingsuite/core/algorithms/AdvancedFilters
    ../../../../../imagingsuite/GUI/qt/QtAddons/
    ../../../../../imagingsuite/GUI/qt/QtModuleConfigure/
    ../../../../../imagingsuite/core/kipl/kipl/include
    ../../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../../imagingsuite/core/modules/ReaderConfig
    ../../../../../ExternalDependencies/macos/include
    ../ImagingModules/src
    ../../ProcessFramework/include/
    # ../../../../external/mac/include/
    # ../../../../external/mac/include/nexus
    # ../../../../external/mac/include/hdf5

    )

link_directories( ../../../../../install/lib /opt/local/lib )

set(INC_FILES
    src/WaveletRingCleanDlg.h
    src/bblognormdlg.h
    src/imagingmodulesgui.h
    src/imagingmodulesgui_global.h
    src/morphspotcleandlg.h
)

set(SRC_FILES
    src/WaveletRingCleanDlg.cpp
    src/bblognormdlg.cpp
    src/imagingmodulesgui.cpp
    src/morphspotcleandlg.cpp
)

set(UI_FILES
    src/WaveletRingCleanDlg.ui
    src/bblognormdlg.ui
    src/morphspotcleandlg.ui
)

add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
  ${UI_FILES}
)


set_target_properties(${TARGET_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_RPATH "\$ORIGIN")
set_target_properties(${TARGET_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

# target_compile_definitions(${TARGET_NAME} PRIVATE INSPECTORMODULESGUI_LIBRARY HAVE_NEXUS)

add_definitions(-DHAVE_NEXUS)

message(${CMAKE_HOST_SYSTEM_PROCESSOR})

link_directories(../../../../../install/lib /opt/local/lib )

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")


    target_link_directories(${TARGET_NAME} PRIVATE
        /opt/local/lib
        ../../../../../ExternalDependencies/macos/${CMAKE_HOST_SYSTEM_PROCESSOR}/lib
    )


    target_link_libraries(${TARGET_NAME}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Charts
    #    Qt${QT_VERSION_MAJOR}::PrintSupport
        kipl
        ImagingAlgorithms
        ImagingModules
        AdvancedFilterModules
        AdvancedFilters
        ModuleConfig
        ReaderConfig
        ProcessFramework
        QtAddons
        QtModuleConfigure
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

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries(${TARGET_NAME} PRIVATE

      Qt${QT_VERSION_MAJOR}::Core
      Qt${QT_VERSION_MAJOR}::Widgets
      Qt${QT_VERSION_MAJOR}::Charts
  #    Qt${QT_VERSION_MAJOR}::PrintSupport
      kipl
      ImagingAlgorithms
      ModuleConfig
      ReconFramework
      QtAddons
      QtModuleConfigure
      xml2
      fftw3
      fftw3f
      cfitsio
      tiff
      armadillo
      lapack
      blas
      # NeXus.1.0.0
      # NeXusCPP.1.0.0
  )
endif()


install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# Generated from QtKipTool.pro.

#####################################################################
## KipTool Binary:
#####################################################################

qt_internal_add_executable(KipTool
    GUI
    SOURCES
        ../../src/Fits2Tif.cpp ../../src/Fits2Tif.h
        ../../src/ImageIO.cpp ../../src/ImageIO.h
        ../../src/ImagingToolConfig.cpp ../../src/ImagingToolConfig.h
        ../../src/Reslicer.cpp ../../src/Reslicer.h
        ../../src/confighistorydialog.cpp ../../src/confighistorydialog.h ../../src/confighistorydialog.ui
        ../../src/fileconversiondialog.cpp ../../src/fileconversiondialog.h ../../src/fileconversiondialog.ui
        ../../src/genericconversion.cpp ../../src/genericconversion.h ../../src/genericconversion.ui
        ../../src/imagingmoduleconfigurator.cpp ../../src/imagingmoduleconfigurator.h
        ../../src/kiptoolmainwindow.cpp ../../src/kiptoolmainwindow.h ../../src/kiptoolmainwindow.ui
        ../../src/loadimagedialog.cpp ../../src/loadimagedialog.h ../../src/loadimagedialog.ui
        ../../src/main.cpp
        ../../src/mergevolume.cpp ../../src/mergevolume.h
        ../../src/mergevolumesdialog.cpp ../../src/mergevolumesdialog.h ../../src/mergevolumesdialog.ui
        ../../src/processdialog.cpp ../../src/processdialog.h ../../src/processdialog.ui
        ../../src/reslicerdialog.cpp ../../src/reslicerdialog.h ../../src/reslicerdialog.ui
    DEFINES
        VERSION=\\\"2.7.0\\\"
    INCLUDE_DIRECTORIES
        ../../../../../imagingsuite/GUI/qt/QtAddons
        ../../../../../imagingsuite/GUI/qt/QtModuleConfigure
        ../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
        ../../../../../imagingsuite/core/kipl/kipl/include
        ../../../../../imagingsuite/core/modules/ModuleConfig/include
        ../../../../../imagingsuite/core/modules/ReaderConfig
        ../../../../../imagingsuite/core/modules/ReaderGUI
        ../../../../frameworks/imageprocessing/ProcessFramework/include
    PUBLIC_LIBRARIES
        ImagingAlgorithms
        ModuleConfig
        ProcessFramework
        Qt::Charts
        Qt::Concurrent
        Qt::Gui
        Qt::Widgets
        QtAddons
        QtModuleConfigure
        ReaderConfig
        ReaderGUI
        kipl
    ENABLE_AUTOGEN_TOOLS
        uic
)

#### Keys ignored in scope 1:.:.:QtKipTool.pro:<TRUE>:
# DEPENDPATH = "$$PWD/../../../../frameworks/imageprocessing/ProcessFramework/include/" "$$REPOS/imagingsuite/GUI/qt/QtAddons" "$$REPOS/imagingsuite/core/modules/ModuleConfig/include" "$$REPOS/imagingsuite/core/modules/ReaderConfig" "$$REPOS/imagingsuite/core/modules/ReaderGUI" "$$REPOS/imagingsuite/core/kipl/kipl/include" "$$REPOS/imagingsuite/core/algorithms/ImagingAlgorithms/src"
# DISTFILES = "../../deploykiptool_mac.sh" "../../deploykiptool_ubuntu.sh" "../../deploykiptool_win.bat"
# ICON = "kiptool_icon.icns"
# RC_ICONS = "kiptool_icon.ico"
# TEMPLATE = "app"

## Scopes:
#####################################################################

qt_internal_extend_target(KipTool CONDITION CMAKE_BUILD_TYPE STREQUAL Release
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/../../../../../lib/
)

#### Keys ignored in scope 2:.:.:QtKipTool.pro:(CMAKE_BUILD_TYPE STREQUAL Release):
# DESTDIR = "$$REPOS/Applications"

qt_internal_extend_target(KipTool CONDITION CMAKE_BUILD_TYPE STREQUAL Debug AND NOT CMAKE_BUILD_TYPE STREQUAL Release
    PUBLIC_LIBRARIES
        # Remove: L/lib/debug/
)

#### Keys ignored in scope 4:.:.:QtKipTool.pro:(CMAKE_BUILD_TYPE STREQUAL Debug):
# DESTDIR = "$$REPOS/Applications/debug"

qt_internal_extend_target(KipTool CONDITION UNIX AND NOT symbian
    PUBLIC_LIBRARIES
        TIFF::TIFF
        cfitsio
        xml2
)

#### Keys ignored in scope 5:.:.:QtKipTool.pro:UNIX AND NOT symbian:
# INSTALLS = "target"

#### Keys ignored in scope 6:.:.:QtKipTool.pro:maemo5:
# target.path = "/opt/usr/lib"

#### Keys ignored in scope 7:.:.:QtKipTool.pro:else:
# target.path = "/usr/lib"

qt_internal_extend_target(KipTool CONDITION MACOS AND NOT symbian
    INCLUDE_DIRECTORIES
        /opt/local/include
        /opt/local/include/libxml2
    COMPILE_OPTIONS
        -O2
        -fPIC
)

#### Keys ignored in scope 8:.:.:QtKipTool.pro:UNIX AND MACOS:
# QMAKE_LIBDIR = "/opt/local/lib"

qt_internal_extend_target(KipTool CONDITION UNIX AND NOT MACOS AND NOT symbian
    INCLUDE_DIRECTORIES
        /usr/include/cfitsio
        /usr/include/libxml2
    PUBLIC_LIBRARIES
        gomp
    COMPILE_OPTIONS
        -O2
        -fPIC
        -fopenmp
    LINK_OPTIONS
        "-lgomp"
)

qt_internal_extend_target(KipTool CONDITION WIN32
    INCLUDE_DIRECTORIES
        ../../../../../imagingsuite/external/include
        ../../../../../imagingsuite/external/include/cfitsio
        ../../../../../imagingsuite/external/include/libxml2
    PUBLIC_LIBRARIES
        cfitsio
        libtiff
        libxml2_dll
    COMPILE_OPTIONS
        /O2
        /openmp
)

#### Keys ignored in scope 10:.:.:QtKipTool.pro:WIN32:
# QMAKE_LIBDIR = "$$REPOS/imagingsuite/external/lib64"

qt_internal_extend_target(KipTool CONDITION (WIN32) AND (QMAKE_HOST.arch___contains___x86_64)
    LINK_OPTIONS
        "/MACHINE:X64"
)

qt_internal_extend_target(KipTool CONDITION (UNIX AND APPLE) AND (EXISTS _ss_REPOS/imagingsuite/external/mac/lib/_x_NeXus_x_)
    DEFINES
        HAVE_NEXUS
    INCLUDE_DIRECTORIES
        /imagingsuite/external/mac/include
        /imagingsuite/external/mac/include/hdf5
        /imagingsuite/external/mac/include/nexus
    PUBLIC_LIBRARIES
        NeXus.1.0.0
        NeXusCPP.1.0.0
)

#### Keys ignored in scope 13:.:.:QtKipTool.pro:EXISTS _ss_REPOS/imagingsuite/external/mac/lib/_x_NeXus_x_:
# DEPENDPATH = "$$REPOS/imagingsuite/external/mac/include/" "$$REPOS/imagingsuite/external/mac/include/nexus" "$$REPOS/imagingsuite/external/mac/include/hdf5"
# QMAKE_LIBDIR = "$$REPOS/imagingsuite/external/mac/lib/"
